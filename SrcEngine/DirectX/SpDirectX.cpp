#include "SpDirectX.h"
#include "SpSwapChainManager.h"
#include "SpDepth.h"
#include "GPipeline.h"
#include "SpRootSignature.h"
#include "SpConstBuffer.h"
#include "SpTextureManager.h"
#include "RTVManager.h"

static SpDirectX WDX;

SpDirectX* GetWDX()
{
	return &WDX;
}

void InitWDX()
{
	WDX.Init();
}

void SpDirectX::Init() {
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
}

bool SpDirectX::StartFrame()
{
	return GetSpWindow()->ProcessMessage();
}

void SpDirectX::PreDrawCommands()
{
	//バックバッファ番号を取得(0か1)
	UINT bbIndex = GetSCM()->swapchain->GetCurrentBackBufferIndex();

	//画面クリア
	Float4 clearColor = { 0.0f, 0.f, 0.f, 0.0f };
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	RTVManager::SetRenderTargetToBackBuffer(bbIndex);
	RTVManager::ClearCurrentRenderTarget(clearColor);
	GetWDX()->cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);

	RTVManager::SetRenderTargetToBackBuffer(bbIndex);
	/*描画処理*/
	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	//ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get()};
	//GetWDX()->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//GetWDX()->cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* ppSrvHeap[] = { SpTextureManager::GetInstance().srvHeap.Get()};
	GetWDX()->cmdList->SetDescriptorHeaps(1, ppSrvHeap);

	D3D12_VIEWPORT viewport{};

	viewport.Width = GetSpWindow()->width;
	viewport.Height = GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // 切り抜き座標下

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);
}

void SpDirectX::PreDraw3D()
{
	/*描画処理*/
	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = GetSpWindow()->width;
	viewport.Height = GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // 切り抜き座標下

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);
}

void SpDirectX::PostDrawCommands()
{
	//リソースバリアーを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	cmdList->ResourceBarrier(1, &barrierDesc);

	RTVManager::GetInstance().isAllResBarClosed = true;
}

void SpDirectX::EndFrame()
{	
	//命令を実行して描画
	cmdList->Close();

	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	GetSCM()->swapchain->Present(1, 0);

	GetSCM()->WaitForRender();
}
