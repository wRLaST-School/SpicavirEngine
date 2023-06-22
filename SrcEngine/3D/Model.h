#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
#include "Vec3.h"
#include "SpTextureManager.h"
#include "SpConstBuffer.h"
#include <exc_unordered_map.h>

struct ModelConsts {
	const static int MAX_BONES_PER_VERTEX = 4;
	const static int MAX_BONES_PER_MODEL = 128;
};

struct Vertex
{
	Float3 pos;
	Float3 normal;
	Float2 uv;
	eastl::array <int32_t, ModelConsts::MAX_BONES_PER_VERTEX> bIndices;
	Float4 bWeights;
};

struct Material
{
	std::string name;
	Float3 ambient;
	Float3 diffuse;
	Float3 specular;
	float alpha;
	TextureKey textureKey;

	Material() {
		ambient = {0.3f, 0.3f, 0.3f};
		diffuse = {0.0f, 0.0f, 0.0f};
		specular = {0.0f, 0.0f, 0.0f};
		alpha = 1.0f;
	}
};

struct ConstBufferDataMaterial {
	Float3 ambient;
	float padding1;
	Float3 diffuse;
	float padding2;
	Float3 specular;
	float alpha;
};

struct ConstBufferDataBoneMatrix {
	Matrix bMatrix[ModelConsts::MAX_BONES_PER_MODEL];
};

namespace ModelCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
}

class Model
{
public:
	Model();
	Model(const string& modelName);
	Model(const string& fileName, bool useAssimp);

	void LoadMaterial(const string& path, const string& filename);
	void UpdateMaterial();

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;

	vector<Material> material;

	vector<SpConstBuffer<ConstBufferDataMaterial>> materialCBs;
	SpConstBuffer<ConstBufferDataBoneMatrix> bMatrixCB;

	Model operator= (Model& m) = delete;
};

typedef std::string ModelKey;

class ModelManager
{
public:
	static void Register(const string& modelName, const ModelKey& key);
	static void Register(const string& modelPath, const ModelKey& key, bool useAssimp);

	static Model* GetModel(const ModelKey& key);

	static void ReleasePerSceneModel();

	static void ReleaseAllModels();

	static void PreLoadNewScene();

private:
	static exc_unordered_map<ModelKey, Model> sModels;
	static list<ModelKey> sPerSceneModels[2];
	static int32_t sCurrentSceneResIndex;
};