#pragma once
#include "Essentials.h"
#include "wDirectX.h"
#include "Vec3.h"
#include "wTextureManager.h"
#include "wConstBuffer.h"
//TODO:頂点データを読み込めるようにする

struct Vertex
{
	Float3 pos;
	Float3 normal;
	Float2 uv;
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
		ambient = {0.3, 0.3, 0.3};
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
			}
	};
}

class Model
{
public:
	Model();
	Model(string modelName);

	void LoadMaterial(const string& path, const string& filename);
	void UpdateMaterial();

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;

	Material material;

	wConstBuffer<ConstBufferDataMaterial> materialCB;
};