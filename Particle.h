#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
#include "Vec3.h"
#include "SpTextureManager.h"
#include "SpConstBuffer.h"

class Particle
{
public:
	Particle();
	void Update();
	void Draw();

	struct Vertex {
		Float3 position;
	};

private:
	const int vertexCount = 1;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	vector<Vertex> vertices;

	SpConstBuffer<Float4> brightness;

	TextureKey texture = "notexture";
};

namespace ParticleCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
	};
}