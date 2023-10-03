#pragma once
#include "Essentials.h"
class SpSwapChainManager
{
public:
	//TODO:�����RTV�̕���RT2��Vector�ɕς���ART��Resource���݌v��
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers = std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>(BB_NUM);

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain = nullptr;

	
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

public:
	void Init();
	void WaitForRender();

	void ResizeAllBuffers();

	static const int32_t BB_NUM = 2;

private:
};

void InitSpSCM();
SpSwapChainManager* GetSCM();