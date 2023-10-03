#include "Sprite.h"
#include <fstream>
#include <sstream>
#include "SpDirectX.h"
#include "GPipeline.h"
#include "SpRootSignature.h"
#include <RootSignatureManager.h>
#include <SpRenderer.h>

Sprite::Sprite(const TextureKey& key)
{
	this->tex = key;

	DirectX::TexMetadata texmeta = SpTextureManager::GetTextureMetadata(tex);
	this->width = (float)texmeta.width;
	this->height = (float)texmeta.height;
	float hlfw = (float)texmeta.width / 2;
	float hlfh = (float)texmeta.height / 2;
	Sprite::Vertex vertices[] = {
		{{-hlfw, hlfh, 0}, {0.0f, 1.0f}},
		{{-hlfw, -hlfh, 0}, {0.0f, 0.0f}},
		{{hlfw, hlfh, 0}, {1.0f, 1.0f}},
		{{hlfw, -hlfh, 0}, {1.0f, 0.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(Sprite::Vertex) * _countof(vertices));

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

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"SPRITE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	Sprite::Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Sprite::Vertex);

	constBuff.contents->color = {1.0, 1.0, 1.0, 1.0};
}

Sprite::Sprite(const std::string& path, const TextureKey& newKey)
{
	this->tex = SpTextureManager::LoadTexture(path, newKey);

	DirectX::TexMetadata texmeta = SpTextureManager::GetTextureMetadata(tex);
	this->width = (float)texmeta.width;
	this->height = (float)texmeta.height;
	float hlfw = (float)texmeta.width / 2;
	float hlfh = (float)texmeta.height / 2;
	Sprite::Vertex vertices[] = {
		{{-hlfw, hlfh, 0}, {0.0f, 1.0f}},
		{{-hlfw, -hlfh, 0}, {0.0f, 0.0f}},
		{{hlfw, hlfh, 0}, {1.0f, 1.0f}},
		{{hlfw, -hlfh, 0}, {1.0f, 0.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(Sprite::Vertex) * _countof(vertices));

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

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"SPRITE VERT BUFF");
	// GPU上のバッファに対応した仮想メモリを取得
	Sprite::Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Sprite::Vertex);

	constBuff.contents->color = { 1.0, 1.0, 1.0, 1.0 };
}

void Sprite::PreSpriteDraw()
{
	ID3D12GraphicsCommandList* cl = GetSpDX()->cmdList.Get();
	cl->SetGraphicsRootSignature(SpRootSignature::Get("2D")->rootsignature.Get());
	cl->SetPipelineState(GPipeline::GetState("2d"));
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D12DescriptorHeap* ppHeaps[] = { SpTextureManager::GetInstance().srvHeap.Get() };
	cl->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::PostSpriteDraw()
{
}

void Sprite::UpdateMatrix()
{
	world = Matrix::Identity();

	world *= Matrix::Scale(scale);
	world *= Matrix::RotZ(rot);
	world *= Matrix::Translation(position);

	constBuff.contents->mat = world;

	constBuff.contents->color = brightness;
}

void Sprite::Draw()
{
	constBuff.contents->mat = constBuff.contents->mat * sProj;
	SpRenderer::DrawCommand([&] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(tex));
		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &vbView);

		GetSpDX()->cmdList->DrawInstanced(4, 1, 0, 0);
		}, SpRenderer::Stage::Sprite);
}

void Sprite::InitCommon()
{
	sProj = Matrix::Projection(GetSpWindow()->width, GetSpWindow()->height);
}


Matrix Sprite::sProj;