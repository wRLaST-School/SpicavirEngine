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

	CD3DX12_HEAP_PROPERTIES htp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES* phtp = &htp;

	CD3DX12_CLEAR_VALUE clval(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	CD3DX12_CLEAR_VALUE* pclval = &clval;

	GetSpDX()->dev->CreateCommittedResource(
		phtp,
		D3D12_HEAP_FLAG_NONE,
		&dResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		pclval,
		IID_PPV_ARGS(&depthBuffer)
	);
	depthBuffer->SetName(L"DEPTH BUFF");

	//cr HeapDesc
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	GetSpDX()->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	GetSpDX()->dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void SpDepth::Resize()
{
	depthBuffer->Release();
	Init();
}

void InitSpDepth()
{
	wdp.Init();
}

SpDepth* GetWDepth()
{
	return &wdp;
}
