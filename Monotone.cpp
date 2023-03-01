#include "stdafx.h"
#include "Monotone.h"
#include <SpShader.h>
#include <GPipeline.h>
#include <SpRootSignature.h>
#include <Sprite.h>

string Monotone::name = "Monotone";

void Monotone::Init()
{
	RegisterRS(name);
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

	pl2dDesc.Blend.Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Add);

	GPipeline::Create(pl2dDesc, name);
}

void Monotone::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}
