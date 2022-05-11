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
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;

private:
};

wDirectX* GetWDX();
void InitWDX();

