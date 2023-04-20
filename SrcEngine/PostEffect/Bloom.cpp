#include "stdafx.h"
#include "Bloom.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <SpSwapChainManager.h>
#include <Sprite.h>


string BloomP1::name = "BloomP1";
string BloomP2::name = "BloomP2";
string BloomP3::name = "BloomP3";
string BloomFin::name = "Finalize";

void BloomP1::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);
}

void BloomP1::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}

void BloomP2::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();
}

void BloomP2::Effect(TextureKey baseTex, TextureKey targetTex)
{
	*cb.contents = GetGaussianWeight(strength);

	IPostEffector::Effect(baseTex, targetTex, name, [&](){GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress()); });
}

void BloomP3::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);
}

void BloomP3::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name, [&](){GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, BloomP2::cb.buffer->GetGPUVirtualAddress()); });
}

void BloomFin::Init()
{
	RegisterRS(name);

	RegisterPipeline(name);

	RegisterShader(name+string("Add"));
	InitVS(name+string("Add"), name + "VS.hlsl");
	InitPS(name+string("Add"), name + "PS.hlsl");

	PipelineDesc pl2dDesc;
	pl2dDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDesc.RootSignature.ptr = SpRootSignature::Get(name)->rootsignature.Get();

	pl2dDesc.Shader.pShader = GetShader(name+string("Add"));

	pl2dDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	pl2dDesc.Blend[0].Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Add);

	GPipeline::Create(pl2dDesc, name+string("Add"));
}

void BloomFin::Effect(TextureKey baseTex, TextureKey p3Tex, TextureKey targetTex)
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
		TexMetadata md = SpTextureManager::GetTextureMetadata(targetTex);
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
	
	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &PostEffectCommon::vbView);

	GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);

	cl->SetPipelineState(GPipeline::GetState(name + string("Add")));

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(p3Tex));

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &PostEffectCommon::vbView);

	GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);
}

GaussianWeight GetGaussianWeight(float strength)
{
	float total = 0;
	std::function<float(float)> GetWeight = [&strength, &total](float x)
	{
		float t = expf(-0.5f * (x * x) / strength);
		total += 2.f * t;
		return t;
	};

	GaussianWeight ret{
		GetWeight(1.f),
		GetWeight(2.f),
		GetWeight(3.f),
		GetWeight(4.f),
		GetWeight(5.f),
		GetWeight(6.f),
		GetWeight(7.f),
		GetWeight(8.f)
	};

	return {
		ret.a / total,
		ret.b / total,
		ret.c / total,
		ret.d / total,
		ret.e / total,
		ret.f / total,
		ret.g / total,
		ret.h / total
	};
}

SpConstBuffer<GaussianWeight> BloomP2::cb(true);

float BloomP2::strength = 4.0f;