#include "wSwapChainManager.h"
#include "wWindow.h"
#include "wDirectX.h"

wSwapChainManager scm;

void wSwapChainManager::Init()
{
	swapchainDesc.Width = GetwWindow()->width;
	swapchainDesc.Height = GetwWindow()->height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;
	GetWDX()->dxgiFactory->CreateSwapChainForHwnd(GetWDX()->cmdQueue.Get(), GetwWindow()->hwnd, &swapchainDesc, nullptr, nullptr, &swapchain1);
	swapchain1.As(&swapchain);

	/**/
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	GetWDX()->dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	for (int i = 0; i < 2; i++)
	{
		swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i, GetWDX()->dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		GetWDX()->dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}

	GetWDX()->dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void wSwapChainManager::WaitForRender()
{
	//描画コマンドが終わったら次のフレームの準備
	GetWDX()->cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	GetWDX()->cmdAllocator->Reset();
	GetWDX()->cmdList->Reset(GetWDX()->cmdAllocator.Get(), nullptr);
}

void InitWSCM()
{
	scm.Init();
}

wSwapChainManager* GetSCM()
{
	return &scm;
}