#include "SpSwapChainManager.h"
#include "SpWindow.h"
#include "SpDirectX.h"
#include "RTVManager.h"

SpSwapChainManager scm;

void SpSwapChainManager::Init()
{
	swapchainDesc.Width = GetSpWindow()->width;
	swapchainDesc.Height = GetSpWindow()->height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式(SpSwapChainManager::ResizeAllBuffers()にも同じものを書く)
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;
	GetSpDX()->dxgiFactory->CreateSwapChainForHwnd(GetSpDX()->cmdQueue.Get(), GetSpWindow()->hwnd, &swapchainDesc, nullptr, nullptr, &swapchain1);
	swapchain1.As(&swapchain);

	RTVManager::CreateHeaps();

	for (int32_t i = 0; i < 2; i++)
	{
		swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		GetSpDX()->dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, RTVManager::GetHeapCPUHandle(RTVManager::GetInstance().numRT - 2 + i));
	}

	GetSpDX()->dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void SpSwapChainManager::WaitForRender()
{
	//描画コマンドが終わったら次のフレームの準備
	GetSpDX()->cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	GetSpDX()->cmdAllocator->Reset();
	GetSpDX()->cmdList->Reset(GetSpDX()->cmdAllocator.Get(), nullptr);
}

void SpSwapChainManager::ResizeAllBuffers()
{
	SpWindow* spw = GetSpWindow();
	for (auto& bb : backBuffers)
	{
		bb.Reset();
	}

	swapchain->ResizeBuffers(0, spw->width, spw->height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//リサイズ後にバックバッファーを再生成
	for (int32_t i = 0; i < 2; i++)
	{
		swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		GetSpDX()->dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, RTVManager::GetHeapCPUHandle(RTVManager::GetInstance().numRT - 2 + i));
	}
}

void InitSpSCM()
{
	scm.Init();
}

SpSwapChainManager* GetSCM()
{
	return &scm;
}