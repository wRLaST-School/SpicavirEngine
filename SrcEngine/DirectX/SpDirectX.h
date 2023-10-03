#pragma once
#include "Essentials.h"
#include "SpWindow.h"

class SpDirectX
{
public:
	void Init();

	bool StartFrame(); 

	void PreDrawCommands();

	void PostDrawCommands();

	void EndFrame();

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> dev = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;

	D3D12_RESOURCE_BARRIER barrierDesc{};

private:
};

SpDirectX* GetSpDX();
void InitSpDX();

