#include "Model.h"
#include <fstream>
#include <sstream>
#pragma warning(push)
#pragma warning(disable:26800)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

Model::Model() {
}

Model::Model(const string& modelName)
{
	string path = "Resources/Models/"+modelName+"/";
	string objfile = modelName + ".obj";

	vector<Vertex> vertices;

	vector<UINT> indices;

	ifstream file;
	file.open(path + objfile);
	if (file.fail()) {
		assert(0);
	}

	string line;
	vector<Float3> posList;
	vector<Vec3> normalList;
	vector<Float2> tcList;
	while (getline(file, line)) {
		istringstream lineStream(line);

		string key;
		getline(lineStream, key, ' ');

		if (key == "v")
		{
			Float3 position{};
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.z;

			posList.emplace_back(position);
		}

		if (key == "vt")
		{
			Float2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;
			tcList.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			Vec3 normal{};

			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;

			normalList.emplace_back(normal);
		}

		if (key == "f")
		{
			string indexString;
			while (getline(lineStream, indexString, ' ')) 
			{
				istringstream indexStream(indexString);
				UINT indexPosition;
				indexStream >> indexPosition;

				indexStream.seekg(1, ios_base::cur);
				UINT indexTexcoord;
				indexStream >> indexTexcoord;

				indexStream.seekg(1, ios_base::cur);
				UINT indexNormal;
				indexStream >> indexNormal;

				Vertex vertex{};
				vertex.pos = posList[indexPosition - 1];
				vertex.normal = normalList[indexNormal - 1];
				vertex.uv = tcList[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back((UINT)indices.size());
			}
		}

		if (key == "mtllib")
		{
			string filename;
			lineStream >> filename;

			LoadMaterial(path, filename);
		}

	}
	file.close();

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (uint32_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	//頂点インデックスバッファの生成
	UINT sizeIB = static_cast<UINT>(sizeof(UINT) * indices.size());

	resdesc.Width = sizeIB;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	indexBuff->SetName(L"INDEX BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	uint32_t* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}

Model::Model(const string& filePath, bool useSmoothShading)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded |
		(useSmoothShading ? aiProcess_GenSmoothNormals : aiProcess_GenNormals) |
		aiProcess_FixInfacingNormals
	);

	if (scene == nullptr) {
		OutputDebugStringA(importer.GetErrorString());
		return;
	}

	vector<Float3> posList;
	vector<Vec3> normalList;
	vector<Float2> tcList;
	vector<UINT> indices;
	vector<Vertex> vertices;

	UINT backIndex = 0;

	//ノードごとの処理
	std::function<void(aiNode*)>fNode = [&](aiNode* cur) {
		for (uint32_t  i = 0; i < cur->mNumChildren; i++)
		{
			fNode(cur->mChildren[i]);
		}
		//このノード用の変換行列
		std::function<aiMatrix4x4(aiNode*)>calcMat = [&](aiNode* calcn) {
			if(calcn->mParent)
				return calcMat(calcn->mParent) * calcn->mTransformation;
			
			return calcn->mTransformation;
		};

		aiMatrix4x4 wt = calcMat(cur);

		//ノードごとのメッシュについて
		for (uint32_t k = 0; k < cur->mNumMeshes; k++) {
			//メッシュごとの処理
			uint32_t i = cur->mMeshes[k];
			aiMesh* mesh = scene->mMeshes[i];

			UINT lastMaxIndex = backIndex;

			for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
				//頂点ごと
				aiVector3D vertex = mesh->mVertices[j];
				vertex *= wt;
				posList.push_back({ vertex.x, vertex.y, vertex.z });
				backIndex++;

				//法線
				if (mesh->mNormals)
				{
					aiVector3D norm = mesh->mNormals[j];
					norm.Normalize();
					normalList.emplace_back(norm.x, norm.y, norm.z);
				}
				//UV
				if (mesh->HasTextureCoords(0))
				{
					tcList.push_back({ mesh->mTextureCoords[0][j].x,mesh->mTextureCoords[0][j].y });
				}

				vertices.emplace_back(Vertex{ posList.back(), normalList.back(), tcList.back() });
			}

			for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
				//ポリゴンごと)
				aiFace face = mesh->mFaces[j];
				for (uint32_t l = 0; l < face.mNumIndices; l++) {
					//インデックスごと
					UINT ind = face.mIndices[l];

					indices.emplace_back(ind + lastMaxIndex);
				}
			}
		}
	};

	//ノードごとの処理呼び出し
	fNode(scene->mRootNode);	

	for (uint32_t i = 0; i < scene->mNumMaterials; i++)
	{
		//マテリアルごと
		material.emplace_back();
		Material* mtr = &material.back();
		aiMaterial* aimtr = scene->mMaterials[i];

		aiColor3D temp;
		aiString tempstr;

		aimtr->Get(AI_MATKEY_NAME, tempstr);
		mtr->name = tempstr.C_Str();

		aimtr->Get(AI_MATKEY_COLOR_AMBIENT, temp);
		mtr->ambient = { temp.r, temp.g, temp.b };

		aimtr->Get(AI_MATKEY_COLOR_SPECULAR, temp);
		mtr->specular = { temp.r, temp.g, temp.b };

		aimtr->Get(AI_MATKEY_COLOR_DIFFUSE, temp);
		mtr->diffuse = { temp.r, temp.g, temp.b };

		//Textrue
		aimtr->GetTexture(aiTextureType_DIFFUSE, 0, &tempstr);
		//TODO:埋め込みテクスチャの場合の処理		
		int32_t pti = (int32_t)filePath.find_last_of("\\");
		int32_t pti2 = (int32_t)filePath.find_last_of("/");
		string filedir = filePath.substr(0, max(pti, pti2));

		mtr->textureKey = SpTextureManager::LoadTexture(filedir + string("/") + string(tempstr.C_Str()), string("asmptex:") + filedir + string(tempstr.C_Str()));
	}

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"VERTEX_BUFF_ASMP");

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	//頂点インデックスバッファの生成
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indices.size());

	resdesc.Width = sizeIB;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	indexBuff->SetName(L"INDEX_BUFF_ASMP");

	// GPU上のバッファに対応した仮想メモリを取得
	uint32_t* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;

	UpdateMaterial();
}

void Model::LoadMaterial(const string& path, const string& filename)
{
	std::ifstream file;

	file.open(path + filename);

	if (file.fail()) { assert(0); }

	string line;

	while (getline(file, line)) {
		std::istringstream lineStream(line);

		string key;
		getline(lineStream, key, ' ');

		if (key[0] == '\t') key.erase(key.begin());

		if (key == "newmtl") {
			string str;
			lineStream >> str;
			material.emplace_back();
			material.back().name = str;
		}

		if (key == "Ka") {
			lineStream >> material.back().ambient.x;
			lineStream >> material.back().ambient.y;
			lineStream >> material.back().ambient.z;
		}

		if (key == "Kd") {
			lineStream >> material.back().diffuse.x;
			lineStream >> material.back().diffuse.y;
			lineStream >> material.back().diffuse.z;
		}

		if (key == "Ks") {
			lineStream >> material.back().specular.x;
			lineStream >> material.back().specular.y;
			lineStream >> material.back().specular.z;
		}

		if (key == "map_Kd") {
			string texName;
			lineStream >> texName;
			material.back().textureKey = SpTextureManager::LoadTexture(path + texName, texName);
		}
	}

	UpdateMaterial();
}

void Model::UpdateMaterial()
{
	materialCBs.emplace_back();
	materialCBs.back().contents->ambient = material.front().ambient;
	materialCBs.back().contents->diffuse = material.front().diffuse;
	materialCBs.back().contents->specular = material.front().specular;
	materialCBs.back().contents->alpha = material.front().alpha;
}

void ModelManager::Register(const string& modelName, const ModelKey& key)
{
	models.Access(
		[&](auto& map) {
			map.emplace(key, modelName);
		}
	);
}

void ModelManager::Register(const string& modelPath, const ModelKey& key, bool useAssimp)
{
	models.Access(
		[&](auto& map) {
			map.emplace(std::piecewise_construct,
				std::forward_as_tuple(key),
				std::forward_as_tuple(modelPath, useAssimp));
		}
	);
}

Model* ModelManager::GetModel(const ModelKey& key)
{
	Model* ret;
	models.Access(
		[&](auto& map) {
			ret = &map.find(key)->second;
		}
	);

	return ret;
}

void ModelManager::ReleasePerSceneModel()
{
	int32_t lastSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
	for (auto itr = perSceneModels[lastSceneResIndex].begin(); itr != perSceneModels[lastSceneResIndex].end(); itr++)
	{
		bool usingInCurrentScene = false;
		for (auto key : perSceneModels[currentSceneResIndex])
		{
			if (key == *itr)
			{
				usingInCurrentScene = true;
			}
		}

		if (!usingInCurrentScene) //今のシーンで使われていないならリリース
		{
			models.Access(
				[&](auto& map) {
					map.erase(*itr);
				}
			);
		}
	}

	perSceneModels[lastSceneResIndex].clear();
}

void ModelManager::ReleaseAllModels()
{
	models.clear();
}

void ModelManager::PreLoadNewScene()
{
	currentSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
}


exc_unordered_map<ModelKey, Model> ModelManager::models;
list<ModelKey> ModelManager::perSceneModels[2];
int32_t ModelManager::currentSceneResIndex = 0;