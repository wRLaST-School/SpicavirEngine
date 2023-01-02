#include "Model.h"
#include <fstream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
Model::Model() {

	//Vertex vertices[] = {
	//	//前
	//	{{-50, -50, -50},{}, {0.0f, 1.0f}},
	//	{{-50,  50, -50},{}, {0.0f, 0.0f}},
	//	{{ 50, -50, -50},{}, {1.0f, 1.0f}},
	//	{{ 50,  50, -50},{}, {1.0f, 0.0f}},

	//	//後				 
	//	{{-50, -50,  50},{}, {0.0f, 1.0f}},
	//	{{-50,  50,  50},{}, {0.0f, 0.0f}},
	//	{{ 50, -50,  50},{}, {1.0f, 1.0f}},
	//	{{ 50,  50,  50},{}, {1.0f, 0.0f}},

	//	//左				 
	//	{{-50, -50, -50},{}, {0.0f, 1.0f}},
	//	{{-50, -50,  50},{}, {0.0f, 0.0f}},
	//	{{-50,  50, -50},{}, {1.0f, 1.0f}},
	//	{{-50,  50,  50},{}, {1.0f, 0.0f}},

	//	//右				 
	//	{{ 50, -50, -50},{}, {0.0f, 1.0f}},
	//	{{ 50, -50,  50},{}, {0.0f, 0.0f}},
	//	{{ 50,  50, -50},{}, {1.0f, 1.0f}},
	//	{{ 50,  50,  50},{}, {1.0f, 0.0f}},

	//	//下				 
	//	{{-50, -50, -50},{}, {0.0f, 1.0f}},
	//	{{ 50, -50, -50},{}, {1.0f, 1.0f}},
	//	{{-50, -50,  50},{}, {0.0f, 0.0f}},
	//	{{ 50, -50,  50},{}, {1.0f, 0.0f}},

	//	//上				 
	//	{{-50,  50, -50},{}, {0.0f, 1.0f}},
	//	{{ 50,  50, -50},{}, {1.0f, 1.0f}},
	//	{{-50,  50,  50},{}, {0.0f, 0.0f}},
	//	{{ 50,  50,  50},{}, {1.0f, 0.0f}},
	//};

	//////頂点インデックス
	//unsigned int indices[] =
	//{
	//	0,1,2,
	//	2,1,3,

	//	6,5,4,
	//	5,6,7,

	//	8,9,10,
	//	11,10,9,

	//	14,13,12,
	//	13,14,15,

	//	16,17,18,
	//	19,18,17,

	//	22,21,20,
	//	21,22,23,
	//};

	//for (int i = 0; i < _countof(indices) / 3; i++)
	//{
	//	UINT i0 = indices[i * 3 + 0];
	//	UINT i1 = indices[i * 3 + 1];
	//	UINT i2 = indices[i * 3 + 2];

	//	Vec3 p0 = Vec3(vertices[i0].pos.x, vertices[i0].pos.y, vertices[i0].pos.z);
	//	Vec3 p1 = Vec3(vertices[i1].pos.x, vertices[i1].pos.y, vertices[i1].pos.z);
	//	Vec3 p2 = Vec3(vertices[i2].pos.x, vertices[i2].pos.y, vertices[i2].pos.z);

	//	Vec3 v1 = p1 - p0;
	//	Vec3 v2 = p2 - p0;

	//	Vec3 normal = v1.Cross(v2);

	//	normal.Norm();
	//	vertices[i0].normal = normal;
	//	vertices[i1].normal = normal;
	//	vertices[i2].normal = normal;
	//}

	//UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

	//////頂点バッファの設定
	//D3D12_HEAP_PROPERTIES heapprop{};
	//heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//D3D12_RESOURCE_DESC resdesc{};
	//resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resdesc.Width = sizeVB;
	//resdesc.Height = 1;
	//resdesc.DepthOrArraySize = 1;
	//resdesc.MipLevels = 1;
	//resdesc.SampleDesc.Count = 1;
	//resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GetWDX()->dev->CreateCommittedResource(
	//	&heapprop,
	//	D3D12_HEAP_FLAG_NONE,
	//	&resdesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff)
	//);

	//// GPU上のバッファに対応した仮想メモリを取得
	//Vertex* vertMap = nullptr;
	//vertBuff->Map(0, nullptr, (void**)&vertMap);

	//// 全頂点に対して
	//for (int i = 0; i < _countof(vertices); i++)
	//{
	//	vertMap[i] = vertices[i];   // 座標をコピー
	//}

	//// マップを解除
	//vertBuff->Unmap(0, nullptr);

	//// 頂点バッファビューの作成
	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeVB;
	//vbView.StrideInBytes = sizeof(Vertex);

	////頂点インデックスバッファの生成
	//UINT sizeIB = static_cast<UINT>(sizeof(unsigned int) * _countof(indices));

	//resdesc.Width = sizeIB;

	//GetWDX()->dev->CreateCommittedResource(
	//	&heapprop,
	//	D3D12_HEAP_FLAG_NONE,
	//	&resdesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff)
	//);

	//// GPU上のバッファに対応した仮想メモリを取得
	//unsigned int* indexMap = nullptr;
	//indexBuff->Map(0, nullptr, (void**)&indexMap);

	//// 全indexに対して
	//for (int i = 0; i < _countof(indices); i++)
	//{
	//	indexMap[i] = indices[i];   // 座標をコピー
	//}

	//// マップを解除
	//indexBuff->Unmap(0, nullptr);

	//// 頂点インデックスバッファビューの作成
	//ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;
}

Model::Model(string modelName)
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
	for (int i = 0; i < vertices.size(); i++)
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
	unsigned int* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int i = 0; i < indices.size(); i++)
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

Model::Model(string filePath, bool ignore)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_FixInfacingNormals |
		aiProcess_FlipUVs
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
		for (int i = 0; i < cur->mNumChildren; i++)
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
		for (int k = 0; k < cur->mNumMeshes; k++) {
			//メッシュごとの処理
			int i = cur->mMeshes[k];
			aiMesh* mesh = scene->mMeshes[i];

			UINT lastMaxIndex = backIndex;

			for (int j = 0; j < mesh->mNumVertices; j++) {
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

			for (int j = 0; j < mesh->mNumFaces; j++) {
				//ポリゴンごと)
				aiFace face = mesh->mFaces[j];
				for (int k = 0; k < face.mNumIndices; k++) {
					//インデックスごと
					UINT ind = face.mIndices[k];

					indices.emplace_back(ind + lastMaxIndex);
				}
			}
		}
	};

	//ノードごとの処理呼び出し
	fNode(scene->mRootNode);	

	for (int i = 0; i < scene->mNumMaterials; i++)
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
		int pti = filePath.find_last_of("\\");
		int pti2 = filePath.find_last_of("/");
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
	for (int i = 0; i < vertices.size(); i++)
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
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned int) * indices.size());

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
	unsigned int* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int i = 0; i < indices.size(); i++)
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

void ModelManager::Register(string modelName, ModelKey key)
{
	models.Access(
		[&](auto& map) {
			map.emplace(key, modelName);
		}
	);
}

void ModelManager::Register(string modelPath, ModelKey key, bool useAssimp)
{
	models.Access(
		[&](auto& map) {
			map.emplace(std::piecewise_construct,
				std::forward_as_tuple(key),
				std::forward_as_tuple(modelPath, true));
		}
	);
}

Model* ModelManager::GetModel(ModelKey key)
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
	int lastSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
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
int ModelManager::currentSceneResIndex = 0;