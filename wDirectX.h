#pragma once
#include "Essentials.h"
#include "wWindow.h"

class wDirectX
{
public:
	void Init();

	void EndFrame();

	

	HRESULT result;
	ComPtr<ID3D12Device> dev = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapchain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
	ComPtr<ID3D12Fence> fence = nullptr;



	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	vector<ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(2);

private:
	UINT64 fenceVal = 0;
};

wDirectX* GetWDX();
void InitWDX();

