#include "wDirectX.h"
#include "wSwapChainManager.h"
#include "wDepth.h"
#include "wPSO.h"
#include "wRootSignature.h"
#include "wConstBuffer.h"
#include "wTextureManager.h"
#include "RTVManager.h"

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

	//�O���{�N�̖ʐڂ��܁`�`��
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	for (auto& adp : adapters)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adp->GetDesc1(&adesc);

		//�\�t�g�E�F�A�͂��ĂтłȂ��̂ł��F�胁�[��
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		wstring strDesc = adesc.Description;
		// Intel UHD�͂���Ȃ�
		if (strDesc.find(L"Intel") == wstring::npos)
		{
			//�N�D�G���˂��`�̗p��
			tmpAdapter = adp;
			break;
		}
	}

	//�f�o�C�X����(�����������Ȃ��悤�ɁI)
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
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}

	//�R�}���h���X�g�ƃR�}���h�L���[
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));

	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

bool wDirectX::StartFrame()
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

void wDirectX::PreDrawCommands()
{
	//�o�b�N�o�b�t�@�ԍ����擾(0��1)
	UINT bbIndex = GetSCM()->swapchain->GetCurrentBackBufferIndex();

	//��ʃN���A
	Float4 clearColor = { 0.1f, 0.25f, 0.5f, 0.0f };
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	RTVManager::SetRenderTargetToBackBuffer(bbIndex);
	RTVManager::ClearCurrentRenderTarget(clearColor);
	GetWDX()->cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);

	RTVManager::SetRenderTargetToBackBuffer(bbIndex);
	/*�`�揈��*/
	GetWDX()->cmdList->SetPipelineState(GetPSO("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(GetRootSignature()->rootsignature.Get());

	//ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get()};
	//GetWDX()->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//GetWDX()->cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* ppSrvHeap[] = { wTextureManager::GetInstance().srvHeap.Get()};
	GetWDX()->cmdList->SetDescriptorHeaps(1, ppSrvHeap);

	D3D12_VIEWPORT viewport{};

	viewport.Width = GetwWindow()->width;
	viewport.Height = GetwWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // �؂蔲�����W��
	scissorrect.right = scissorrect.left + GetwWindow()->width;        // �؂蔲�����W�E
	scissorrect.top = 0;                                        // �؂蔲�����W��
	scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // �؂蔲�����W��

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);
}

void wDirectX::PreDraw3D()
{
	/*�`�揈��*/
	GetWDX()->cmdList->SetPipelineState(GetPSO("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(GetRootSignature()->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = GetwWindow()->width;
	viewport.Height = GetwWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // �؂蔲�����W��
	scissorrect.right = scissorrect.left + GetwWindow()->width;        // �؂蔲�����W�E
	scissorrect.top = 0;                                        // �؂蔲�����W��
	scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // �؂蔲�����W��

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);
}

void wDirectX::PostDrawCommands()
{
	//���\�[�X�o���A�[��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	cmdList->ResourceBarrier(1, &barrierDesc);

	RTVManager::GetInstance().isAllResBarClosed = true;
}

void wDirectX::EndFrame()
{	
	//���߂����s���ĕ`��
	cmdList->Close();

	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	GetSCM()->swapchain->Present(1, 0);

	GetSCM()->WaitForRender();
}
