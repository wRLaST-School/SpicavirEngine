#pragma once
#include "Essentials.h"
class SpSwapChainManager
{
public:
	//TODO:‚±‚ê‚ğRTV‚Ì•û‚ÅRT2‚Â‚ÌVector‚É•Ï‚¦‚éART‚ğResource‚ÂİŒv‚É
	vector<ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(bbNum);

	ComPtr<IDXGISwapChain4> swapchain = nullptr;

	
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

public:
	void Init();
	void WaitForRender();

	void ResizeAllBuffers();

	static const int32_t bbNum = 2;

private:
};

void InitWSCM();
SpSwapChainManager* GetSCM();