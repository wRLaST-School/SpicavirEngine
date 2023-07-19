#include "stdafx.h"
#include "IPostEffector.h"
#include <GPipeline.h>
#include <SpRootSignature.h>
#include <Sprite.h>
#include <SpSwapChainManager.h>

using namespace std;

void IPostEffector::RegisterPipeline(const string& name)
{
	RegisterShader(name);
	InitVS(name, name + "VS.hlsl");
	InitPS(name, name + "PS.hlsl");

	PipelineDesc pl2dDesc;
	pl2dDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDesc.RootSignature.ptr = SpRootSignature::Get(name)->rootsignature.Get();

	pl2dDesc.Shader.pShader = GetShader(name);

	pl2dDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(pl2dDesc, name);
}

void IPostEffector::RegisterRS(const string& name)
{
#pragma region 2D Default RS
	{
		SpRootSignature* rs = SpRootSignature::Register(name);

		rs->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番各PostEffect専用データ
		rs->params.emplace_back();
		rs->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs->params[0].Descriptor.ShaderRegister = 0;
		rs->params[0].Descriptor.RegisterSpace = 0;
		rs->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rs->params.emplace_back();
		rs->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rs->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rs->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rs->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rs->Create();
	}
#pragma endregion
}

void IPostEffector::Effect(const TextureKey& baseTex, const TextureKey& targetTex, const string& name, std::function<void(void)> commands)
{
	if (targetTex == "CurrentBuffer")
	{
		RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
	}
	else
	{
		RTVManager::SetRenderTargetToTexture(targetTex);
	}

	ID3D12GraphicsCommandList* cl = GetWDX()->cmdList.Get();
	cl->SetGraphicsRootSignature(SpRootSignature::Get(name)->rootsignature.Get());
	cl->SetPipelineState(GPipeline::GetState(name));
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	commands();

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorrect{};

	if (targetTex == "CurrentBuffer")
	{
		viewport.Width = (FLOAT)GetSpWindow()->width;
		viewport.Height = (FLOAT)GetSpWindow()->height;
		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + (LONG)viewport.Width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + (LONG)viewport.Height;       // 切り抜き座標下
	}
	else
	{
		DirectX::TexMetadata md = SpTextureManager::GetTextureMetadata(targetTex);
		viewport.Width = (FLOAT)md.width;
		viewport.Height = (FLOAT)md.height;
		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + (LONG)md.width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + (LONG)md.height;       // 切り抜き座標下
	}

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	ID3D12DescriptorHeap* ppHeaps[] = { SpTextureManager::GetInstance().srvHeap.Get() };
	cl->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(baseTex));
	//GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &PostEffectCommon::sVbView);

	GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);
}

D3D12_VERTEX_BUFFER_VIEW PostEffectCommon::sVbView{};

Microsoft::WRL::ComPtr<ID3D12Resource> PostEffectCommon::sVertBuff = nullptr;

void PostEffectCommon::Init()
{
	Sprite::Vertex vertices[] = {
			{{-1, 1, 0}, {0.0f, 0.0f}},
			{{-1, -1, 0}, {0.0f, 1.0f}},
			{{1, 1, 0}, {1.0f, 0.0f}},
			{{1, -1, 0}, {1.0f, 1.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(Sprite::Vertex) * _countof(vertices));

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

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sVertBuff)
	);

	sVertBuff->SetName(L"POST EFFECT VERT BUFF");
	// GPU上のバッファに対応した仮想メモリを取得
	Sprite::Vertex* vertMap = nullptr;
	sVertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	sVertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	sVbView.BufferLocation = sVertBuff->GetGPUVirtualAddress();
	sVbView.SizeInBytes = sizeVB;
	sVbView.StrideInBytes = sizeof(Sprite::Vertex);
}
