#include "stdafx.h"
#include "Particle.h"
#include <ParticleManager.h>
#include <SpDirectX.h>
#include <SpTextureManager.h>
#include <Model.h>
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Camera.h>

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

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	//// 全頂点に対して
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	vertMap[i] = vertices[i];   // 座標をコピー
	//}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	*brightness.contents = {1.f, 1.f, 1.f, 1.f};
}

void ParticleManager::Draw()
{
	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("particle"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("Particle")->rootsignature.Get());

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(texture));
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, this->brightness.buffer->GetGPUVirtualAddress());

	Camera::UseCurrent();

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &vbView);

	GetWDX()->cmdList->DrawInstanced(activeCount, 1, 0, 0);
}
