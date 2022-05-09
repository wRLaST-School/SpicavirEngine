#include "Essentials.h"
#include "wWindow.h"
#include "wDirectX.h"
#include "Input.h"

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
	InitWDX();
	HRESULT result;

	//Init Input
	InitInput();

	/*Init Draw*/
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	Vertex vertices[] = {
		//前
		{{-50, -50, 50},{0.0f, 1.0f}},
		{{-50,  50, 50},{0.0f, 0.0f}},
		{{ 50, -50, 50},{1.0f, 1.0f}},
		{{ 50,  50, 50},{1.0f, 0.0f}},
			
		//後
		{{-50, -50,  150},{0.0f, 1.0f}},
		{{-50,  50,  150},{0.0f, 0.0f}},
		{{ 50, -50,  150},{1.0f, 1.0f}},
		{{ 50,  50,  150},{1.0f, 0.0f}},
			
		//左
		{{-50, -50, 50},{0.0f, 1.0f}},
		{{-50, -50,  150},{0.0f, 0.0f}},
		{{-50,  50, 50},{1.0f, 1.0f}},
		{{-50,  50,  150},{1.0f, 0.0f}},
			
		//右
		{{ 50, -50, 50},{0.0f, 1.0f}},
		{{ 50, -50,  150},{0.0f, 0.0f}},
		{{ 50,  50, 50},{1.0f, 1.0f}},
		{{ 50,  50,  150},{1.0f, 0.0f}},
			
		//下
		{{-50, -50, 50},{0.0f, 1.0f}},
		{{ 50, -50, 50},{0.0f, 0.0f}},
		{{-50, -50,  150},{1.0f, 1.0f}},
		{{-50, -50,  150},{1.0f, 0.0f}},
			
		//上
		{{-50,  50, 50},{0.0f, 1.0f}},
		{{ 50,  50, 50},{0.0f, 0.0f}},
		{{-50,  50,  150},{1.0f, 1.0f}},
		{{-50,  50,  150},{1.0f, 0.0f}},
	};

	//Vertex vertices[] = {
	//	//前
	//	{{-50,  50, 50},{0.0f, 1.0f}},
	//	{{-50, -50, 50},{0.0f, 0.0f}},
	//	{{ 50,  50, 50},{1.0f, 1.0f}},
	//	{{ 50, -50, 50},{1.0f, 0.0f}},

	//};

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

	////頂点バッファの設定
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
	result = GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(Vertex);

	////頂点インデックス
	unsigned short indices[] =
	{
		0,1,2,
		1,2,3,

		4,5,6,
		5,6,7,

		8,9,10,
		9,10,11,

		12,13,14,
		13,14,15,

		16,17,18,
		17,18,19,

		20,21,22,
		21,22,23,
	};

	//頂点インデックスバッファの生成
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * _countof(indices));

	ComPtr<ID3D12Resource> indexBuff = nullptr;
	resdesc.Width = sizeIB;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


	////定数バッファ
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; //RGBA
	};

	struct ConstBufferDataTransform {
		XMMATRIX mat;//3D変換行列
	};

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbheapprop{};
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC cbresdesc{};
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPUリソースの生成
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	GetWDX()->dev->CreateCommittedResource(
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));

	//定数バッファ2
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* cBufTransform = nullptr;

	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbheapprop{};
		cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

		//リソース設定
		D3D12_RESOURCE_DESC cbresdesc{};
		cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbresdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
		cbresdesc.Height = 1;
		cbresdesc.DepthOrArraySize = 1;
		cbresdesc.MipLevels = 1;
		cbresdesc.SampleDesc.Count = 1;
		cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//GPUリソースの生成
		ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
		GetWDX()->dev->CreateCommittedResource(
			&cbheapprop,
			D3D12_HEAP_FLAG_NONE,
			&cbresdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));

		HRESULT hr = constBuffTransform->Map(0, nullptr, (void**)&cBufTransform);
		assert(SUCCEEDED(hr));

		cBufTransform->mat = XMMatrixIdentity();
	}

	//定数バッファ用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> basicDescHeap = nullptr;

	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;

	//生成
	GetWDX()->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();


	//定数バッファビューの作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuffMaterial->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuffMaterial->GetDesc().Width;
	GetWDX()->dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

	unique_ptr<ConstBufferDataMaterial> cBufMaterial = nullptr;
	constBuffMaterial->Map(0, nullptr, (void**)cBufMaterial.get());

	//定数バッファここまで

	//画像データの作成
	const int texWidth = 256;
	const int imageDataCount = texWidth * texWidth;

	unique_ptr<XMFLOAT4> imageData = unique_ptr<XMFLOAT4>(new XMFLOAT4[imageDataCount]);

	for (int i = 0; i < imageDataCount; i++)
	{
		imageData.get()[i].x = i % 256 < 128 ? 1.0f : 0.0f;
		imageData.get()[i].y = i % 256 < 128 ? 0.0f : 1.0f;
		imageData.get()[i].z = 0.0f;
		imageData.get()[i].w = 1.0f;
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	LoadFromWICFile(L"Resources/think.png", WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);



	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texresdesc{};
	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = metadata.height;
	texresdesc.MipLevels = metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	ComPtr<ID3D12Resource> texBuff = nullptr;
	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}

	const size_t kMaxSRVCount = 256;
	
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの生成
	basicHeapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

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

		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
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

	D3D12_DESCRIPTOR_RANGE descRange{};
	descRange.NumDescriptors = 1;
	descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRange.BaseShaderRegister = 0;
	descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].Descriptor.RegisterSpace = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].Descriptor.ShaderRegister = 1;
	rootParams[2].Descriptor.RegisterSpace = 0;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	ComPtr<ID3D12RootSignature> rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = GetWDX()->dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	result = GetWDX()->dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
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
		/*更新処理*/


		/*更新処理ここまで*/


		/*描画処理*/
		cBufTransform->mat = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)GetwWindow()->width / GetwWindow()->height,
			0.1f, 1000.0f
		);


		/*描画処理ここまで*/

		//バックバッファ番号を取得(0か1)
		UINT bbIndex = GetWDX()->swapchain->GetCurrentBackBufferIndex();

		//リソースバリアーを書き込み可能状態に
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = GetWDX()->backBuffers[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		GetWDX()->cmdList->ResourceBarrier(1, &barrierDesc);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetWDX()->rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex, GetWDX()->dev->GetDescriptorHandleIncrementSize(GetWDX()->heapDesc.Type));
		GetWDX()->cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		//画面クリア
		float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
		GetWDX()->cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

		/*描画処理*/
		GetWDX()->cmdList->SetPipelineState(pipelinestate.Get());
		GetWDX()->cmdList->SetGraphicsRootSignature(rootsignature.Get());

		//ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get()};
		//GetWDX()->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GetWDX()->cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		ID3D12DescriptorHeap* ppSrvHeap[] = {srvHeap.Get()};
		GetWDX()->cmdList->SetDescriptorHeaps(1, ppSrvHeap);
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

		D3D12_VIEWPORT viewport{};

		viewport.Width = GetwWindow()->width;
		viewport.Height = GetwWindow()->height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		GetWDX()->cmdList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorrect{};

		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + GetwWindow()->width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // 切り抜き座標下

		GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

		GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &vbView);

		GetWDX()->cmdList->IASetIndexBuffer(&ibView);

		GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

		GetWDX()->cmdList->DrawIndexedInstanced(ibView.SizeInBytes / sizeof(short), 1, 0, 0, 0);

		/*描画処理ここまで*/

		//リソースバリアーを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		GetWDX()->cmdList->ResourceBarrier(1, &barrierDesc);

		/*毎フレーム処理ここまで*/
		GetWDX()->EndFrame();
	}
	/*ループここまで*/
	CloseAllwWindow();


	return 0;
}