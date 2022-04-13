#include "wDirectX.h"
static wDirectX WDX;

wDirectX* GetWDX()
{
	return &WDX;
}

void InitWDX()
{
	WDX.Init();
}

void wDirectX::Init() {
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	vector< ComPtr<IDXGIAdapter1>> adapters;

	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;

	//グラボ君の面接しま～～す
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	for (auto& adp : adapters)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adp->GetDesc1(&adesc);

		//ソフトウェアはお呼びでないのでお祈りメール
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		wstring strDesc = adesc.Description;
		// Intel UHDはさよなら
		if (strDesc.find(L"Intel") == wstring::npos)
		{
			//君優秀だねぇ～採用ぅ
			tmpAdapter = adp;
			break;
		}
	}

	//デバイス生成(複数生成しないように！)
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}

	//コマンドリストとコマンドキュー
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));

	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	//ダブルバッファリングとか
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

	swapchainDesc.Width = GetwWindow()->width;
	swapchainDesc.Height = GetwWindow()->height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;
	dxgiFactory->CreateSwapChainForHwnd(cmdQueue.Get(), GetwWindow()->hwnd, &swapchainDesc, nullptr, nullptr, &swapchain1);
	swapchain1.As(&swapchain);

	/**/
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	for (int i = 0; i < 2; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void wDirectX::EndFrame()
{	
	//命令を実行して描画
	cmdList->Close();

	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	swapchain->Present(1, 0);

	//描画コマンドが終わったら次のフレームの準備
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);
}
