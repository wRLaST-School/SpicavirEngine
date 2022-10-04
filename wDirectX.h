#pragma once
#include "Essentials.h"
#include "wWindow.h"

class wDirectX
{
public:
	void Init();

	bool StartFrame(); 
	
	void PreDraw3D();

	void PreDrawCommands();

	void PostDrawCommands();

	void EndFrame();

	HRESULT result;
	ComPtr<ID3D12Device> dev = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;

	D3D12_RESOURCE_BARRIER barrierDesc{};

private:
};

wDirectX* GetWDX();
void InitWDX();

