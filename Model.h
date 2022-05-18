#pragma once
#include "Essentials.h"
#include "wDirectX.h"
#include "Vec3.h"
//TODO:頂点データを読み込めるようにする

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

class Model
{
public:
	Model();
	Model(string path);

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
};

