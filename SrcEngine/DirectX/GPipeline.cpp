#include "GPipeline.h"
#include "SpRootSignature.h"
map<string, GPipeline> psoMap;

void GPipeline::InitDesc(PipelineDesc desc)
{
	// グラフィックスパイプライン設定
	psod.VS.pShaderBytecode = desc.Shader.pShader->vsBlob->GetBufferPointer();
	psod.VS.BytecodeLength = desc.Shader.pShader->vsBlob->GetBufferSize();
	psod.PS.pShaderBytecode = desc.Shader.pShader->psBlob->GetBufferPointer();
	psod.PS.BytecodeLength = desc.Shader.pShader->psBlob->GetBufferSize();

	if (desc.Shader.pShader->gsBlob != nullptr)
	{
		psod.GS.pShaderBytecode = desc.Shader.pShader->gsBlob->GetBufferPointer();
		psod.GS.BytecodeLength = desc.Shader.pShader->gsBlob->GetBufferSize();
	}
	psod.PrimitiveTopologyType = desc.Render.PrimitiveTopologyType;

	psod.SampleMask = desc.Render.SampleMask;
	psod.RasterizerState = desc.Render.RasterizerState;

	D3D12_RENDER_TARGET_BLEND_DESC* blendDesc[8] = {
		& psod.BlendState.RenderTarget[0],
		& psod.BlendState.RenderTarget[1],
		& psod.BlendState.RenderTarget[2],
		& psod.BlendState.RenderTarget[3],
		& psod.BlendState.RenderTarget[4],
		& psod.BlendState.RenderTarget[5],
		& psod.BlendState.RenderTarget[6],
		& psod.BlendState.RenderTarget[7],
	};

	for (int32_t i = 0; i < 8; i++)
	{
		blendDesc[i]->RenderTargetWriteMask = desc.Blend[i].RenderTargetWriteMask;
		blendDesc[i]->BlendEnable = desc.Blend[i].BlendEnable;
		blendDesc[i]->BlendOpAlpha = desc.Blend[i].Desc.BlendOpAlpha;
		blendDesc[i]->SrcBlendAlpha = desc.Blend[i].Desc.SrcBlendAlpha;
		blendDesc[i]->DestBlendAlpha = desc.Blend[i].Desc.DestBlendAlpha;

		blendDesc[i]->BlendOp = desc.Blend[i].Desc.BlendOp;
		blendDesc[i]->SrcBlend = desc.Blend[i].Desc.SrcBlend;
		blendDesc[i]->DestBlend = desc.Blend[i].Desc.DestBlend;
	}

	psod.NumRenderTargets = desc.Render.NumRenderTargets;
	psod.RTVFormats[0] = desc.Render.RTVFormat[0];
	psod.RTVFormats[1] = desc.Render.RTVFormat[1];
	psod.RTVFormats[2] = desc.Render.RTVFormat[2];
	psod.RTVFormats[3] = desc.Render.RTVFormat[3];
	psod.RTVFormats[4] = desc.Render.RTVFormat[4];
	psod.RTVFormats[5] = desc.Render.RTVFormat[5];
	psod.RTVFormats[6] = desc.Render.RTVFormat[6];
	psod.RTVFormats[7] = desc.Render.RTVFormat[7];
	psod.SampleDesc = desc.Render.SampleDesc;

	//Depth
	psod.DepthStencilState = desc.Depth.DepthStencilState;
	psod.DSVFormat = desc.Depth.DSVFormat;

	psod.InputLayout = desc.Render.InputLayout;
	psod.pRootSignature = desc.RootSignature.ptr;
}

//void GPipeline::InitDesc2D()
//{
//	// グラフィックスパイプライン設定
//	psod.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
//	psod.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
//	psod.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
//	psod.PS.BytecodeLength = shader->psBlob->GetBufferSize();
//
//	psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	psod.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
//	psod.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//	psod.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
//
//	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psod.BlendState.RenderTarget[0];
//	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
//	blendDesc.BlendEnable = true;
//	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
//	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
//
//	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
//	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
//	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
//
//	psod.NumRenderTargets = 1; // 描画対象は1つ
//	psod.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
//	psod.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング
//
//	//Depth
//	psod.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
//	psod.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
//	psod.DepthStencilState.DepthEnable = false;
//	psod.pRootSignature = GetRootSignature()->rootsignature.Get();
//}

void GPipeline::Create(PipelineDesc desc, string id)
{
	psoMap[id] = GPipeline();
	GPipeline* ptr = &psoMap[id];
	ptr->InitDesc(desc);
	ptr->Create();
}

void GPipeline::Create()
{
	GetWDX()->dev->CreateGraphicsPipelineState(&psod, IID_PPV_ARGS(&pso));
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC* GPipeline::GetDesc(string id)
{
	return &psoMap[id].psod;
}

ID3D12PipelineState* GPipeline::GetState(string id)
{
	return psoMap[id].pso.Get();
}

GPipeline* GPipeline::GetGPipeline(string id)
{
	return &psoMap[id];
}

PipelineDesc::Blend::BlendDesc PipelineUtil::Blend::GetBlendMode(BlendMode blendMode)
{
	PipelineDesc::Blend::BlendDesc desc;
	switch (blendMode)
	{
	case PipelineUtil::BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case PipelineUtil::BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;
		desc.SrcBlend = D3D12_BLEND_ONE;
		desc.DestBlend = D3D12_BLEND_ONE;
		break;

	case PipelineUtil::BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		desc.SrcBlend = D3D12_BLEND_ONE;
		desc.DestBlend = D3D12_BLEND_ONE;
		break;

	case PipelineUtil::BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;
		desc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		desc.DestBlend = D3D12_BLEND_ZERO;
		break;

	default:
		break;
	}

	return desc;
}
