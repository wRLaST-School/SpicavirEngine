#include "SpDepth.h"
#include "SpWindow.h"
#include "SpDirectX.h"

SpDepth wdp;

void SpDepth::Init()
{
	D3D12_RESOURCE_DESC dResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		GetSpWindow()->width,
		GetSpWindow()->height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	GetWDX()->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&dResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);

	//cr HeapDesc
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	GetWDX()->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	GetWDX()->dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void InitWDepth()
{
	wdp.Init();
}

SpDepth* GetWDepth()
{
	return &wdp;
}
