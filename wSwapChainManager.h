#pragma once
#include "Essentials.h"
class wSwapChainManager
{
public:
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	vector<ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(2);

	ComPtr<IDXGISwapChain4> swapchain = nullptr;

	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

public:
	void Init();
	void WaitForRender();

private:
};

void InitWSCM();
wSwapChainManager* GetSCM();