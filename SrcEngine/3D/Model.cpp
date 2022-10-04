#include "Model.h"
#include <fstream>
#include <sstream>
Model::Model() {

	Vertex vertices[] = {
		//前
		{{-50, -50, -50},{}, {0.0f, 1.0f}},
		{{-50,  50, -50},{}, {0.0f, 0.0f}},
		{{ 50, -50, -50},{}, {1.0f, 1.0f}},
		{{ 50,  50, -50},{}, {1.0f, 0.0f}},

		//後				 
		{{-50, -50,  50},{}, {0.0f, 1.0f}},
		{{-50,  50,  50},{}, {0.0f, 0.0f}},
		{{ 50, -50,  50},{}, {1.0f, 1.0f}},
		{{ 50,  50,  50},{}, {1.0f, 0.0f}},

		//左				 
		{{-50, -50, -50},{}, {0.0f, 1.0f}},
		{{-50, -50,  50},{}, {0.0f, 0.0f}},
		{{-50,  50, -50},{}, {1.0f, 1.0f}},
		{{-50,  50,  50},{}, {1.0f, 0.0f}},

		//右				 
		{{ 50, -50, -50},{}, {0.0f, 1.0f}},
		{{ 50, -50,  50},{}, {0.0f, 0.0f}},
		{{ 50,  50, -50},{}, {1.0f, 1.0f}},
		{{ 50,  50,  50},{}, {1.0f, 0.0f}},

		//下				 
		{{-50, -50, -50},{}, {0.0f, 1.0f}},
		{{ 50, -50, -50},{}, {1.0f, 1.0f}},
		{{-50, -50,  50},{}, {0.0f, 0.0f}},
		{{ 50, -50,  50},{}, {1.0f, 0.0f}},

		//上				 
		{{-50,  50, -50},{}, {0.0f, 1.0f}},
		{{ 50,  50, -50},{}, {1.0f, 1.0f}},
		{{-50,  50,  50},{}, {0.0f, 0.0f}},
		{{ 50,  50,  50},{}, {1.0f, 0.0f}},
	};

	////頂点インデックス
	unsigned short indices[] =
	{
		0,1,2,
		2,1,3,

		6,5,4,
		5,6,7,

		8,9,10,
		11,10,9,

		14,13,12,
		13,14,15,

		16,17,18,
		19,18,17,

		22,21,20,
		21,22,23,
	};

	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		USHORT i0 = indices[i * 3 + 0];
		USHORT i1 = indices[i * 3 + 1];
		USHORT i2 = indices[i * 3 + 2];

		Vec3 p0 = Vec3(vertices[i0].pos.x, vertices[i0].pos.y, vertices[i0].pos.z);
		Vec3 p1 = Vec3(vertices[i1].pos.x, vertices[i1].pos.y, vertices[i1].pos.z);
		Vec3 p2 = Vec3(vertices[i2].pos.x, vertices[i2].pos.y, vertices[i2].pos.z);

		Vec3 v1 = p1 - p0;
		Vec3 v2 = p2 - p0;

		Vec3 normal = v1.Cross(v2);

		normal.Norm();
		vertices[i0].normal = normal;
		vertices[i1].normal = normal;
		vertices[i2].normal = normal;
	}

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

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

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
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
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * _countof(indices));

	resdesc.Width = sizeIB;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

Model::Model(string modelName)
{
	string path = "Resources/Models/"+modelName+"/";
	string objfile = modelName + ".obj";

	vector<Vertex> vertices;

	vector<USHORT> indices;

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
				USHORT indexPosition;
				indexStream >> indexPosition;

				indexStream.seekg(1, ios_base::cur);
				USHORT indexTexcoord;
				indexStream >> indexTexcoord;

				indexStream.seekg(1, ios_base::cur);
				USHORT indexNormal;
				indexStream >> indexNormal;

				Vertex vertex{};
				vertex.pos = posList[indexPosition - 1];
				vertex.normal = normalList[indexNormal - 1];
				vertex.uv = tcList[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back((USHORT)indices.size());
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
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	resdesc.Width = sizeIB;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
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
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
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
			lineStream >> material.name;
		}

		if (key == "Ka") {
			lineStream >> material.ambient.x;
			lineStream >> material.ambient.y;
			lineStream >> material.ambient.z;
		}

		if (key == "Kd") {
			lineStream >> material.diffuse.x;
			lineStream >> material.diffuse.y;
			lineStream >> material.diffuse.z;
		}

		if (key == "Ks") {
			lineStream >> material.specular.x;
			lineStream >> material.specular.y;
			lineStream >> material.specular.z;
		}

		if (key == "map_Kd") {
			string texName;
			lineStream >> texName;
			material.textureKey = wTextureManager::LoadTexture(path + texName, texName);
		}
	}

	UpdateMaterial();
}

void Model::UpdateMaterial()
{
	materialCB.contents->ambient = material.ambient;
	materialCB.contents->diffuse = material.diffuse;
	materialCB.contents->specular = material.specular;
	materialCB.contents->alpha = material.alpha;
}
