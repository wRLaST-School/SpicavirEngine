#include "stdafx.h"
#include "SrAlphaStage.h"
#include <Model.h>

void SrAlphaStage::Init()
{
#pragma region デフォルト3D
	PipelineDesc defDesc;
	defDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	defDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	defDesc.RootSignature.ptr = SpRootSignature::Get("NoLight3D")->rootsignature.Get();

	defDesc.Shader.pShader = GetShader("lightless");
	defDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	defDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	GPipeline::Create(defDesc, "Alpha3D");
#pragma endregion
}
