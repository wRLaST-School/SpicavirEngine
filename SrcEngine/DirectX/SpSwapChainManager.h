#pragma once
#include "Essentials.h"
class SpSwapChainManager
{
public:
	//TODO:これをRTVの方でRT2つのVectorに変える、RTをResource持つ設計に
	vector<ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(BB_NUM);

	ComPtr<IDXGISwapChain4> swapchain = nullptr;

	
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

public:
	void Init();
	void WaitForRender();

	void ResizeAllBuffers();

	static const int32_t BB_NUM = 2;

private:
};

void InitWSCM();
SpSwapChainManager* GetSCM();