#include "Essentials.h"
#include "wWindow.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	/*デバッグ有効化*/
#ifdef  _DEBUG
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif //  _DEBUG

	//ウィンドウを生成
	{
		wWindow wwnd;
		wwnd.Create(L"DirectXTest", 1280, 720);
		RegisterwWindow(wwnd, "Default");
	}

	MSG msg{};

	/*DirectX初期化処理*/
	//Vars
	HRESULT result;
	ComPtr<ID3D12Device> dev = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapchain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	vector< ComPtr<IDXGIAdapter1>> adapters;

	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;

	//グラボ君の面接しま～～す
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	for(auto &adp : adapters)
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
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	vector<ComPtr<ID3D12Resource>> backBuffers(2);

	for (int i = 0; i < 2; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}

	//フェンス
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	/*DirectX初期化処理ここまで*/

	/*Init Draw*/
	XMFLOAT3 vertices[] = {
		{-0.5f, -0.5f, 0 },
		{-0.5f, 0.5f, 0},
		{0.5f, -0.5f, 0},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
		);

	// GPU上のバッファに対応した仮想メモリを取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	//シェーダーの読み込みとコンパイル
	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, 
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	ComPtr<ID3D12RootSignature> rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	ID3D12PipelineState* pipelinestate = nullptr;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	/*Init Draw End*/

	/*ループ*/
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		/*毎フレーム処理*/
		
		//バックバッファ番号を取得(0か1)
		UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

		//リソースバリアーを書き込み可能状態に
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		cmdList->ResourceBarrier(1, &barrierDesc);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		//画面クリア
		float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
		cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

		/*描画処理*/
		cmdList->SetPipelineState(pipelinestate);
		cmdList->SetGraphicsRootSignature(rootsignature.Get());

		D3D12_VIEWPORT viewport{};

		viewport.Width = GetwWindow()->width;
		viewport.Height = GetwWindow()->height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		cmdList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorrect{};

		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + GetwWindow()->width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // 切り抜き座標下

		cmdList->RSSetScissorRects(1, &scissorrect);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->IASetVertexBuffers(0, 1, &vbView);

		cmdList->DrawInstanced(3, 1, 0, 0);

		/*描画処理ここまで*/

		//リソースバリアーを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		cmdList->ResourceBarrier(1, &barrierDesc);

		//命令を実行して描画
		cmdList->Close();

		ID3D12CommandList* cmdLists[] = {cmdList.Get()};
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

		/*毎フレーム処理ここまで*/
	}
	/*ループここまで*/
	CloseAllwWindow();


	return 0;
}