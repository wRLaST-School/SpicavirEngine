#include "stdafx.h"
#include "Particle.h"
#include <ParticleManager.h>
#include <SpDirectX.h>
#include <SpTextureManager.h>
#include <Model.h>
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Camera.h>
#include <SpRenderer.h>

ParticleManager::ParticleManager()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertexCount);

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
	vertBuff->SetName(L"PARTICLE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);
}

void ParticleManager::Draw()
{
	if (activeCount == 0)
	{
		return;
	}

	SpRenderer::DrawCommand([&] {
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(texture));

		Camera::UseCurrent();

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &vbView);

		GetWDX()->cmdList->DrawInstanced(activeCount, 1, 0, 0);
	}, SpRenderer::Stage::Particle);
}
