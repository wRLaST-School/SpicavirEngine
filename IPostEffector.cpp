#include "stdafx.h"
#include "IPostEffector.h"
#include <GPipeline.h>
#include <SpRootSignature.h>
#include <Sprite.h>

void IPostEffector::RegisterPipeline()
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

void IPostEffector::RegisterRS()
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

void IPostEffector::Effect(TextureKey baseTex, TextureKey targetTex)
{
	
}

string IPostEffector::name;