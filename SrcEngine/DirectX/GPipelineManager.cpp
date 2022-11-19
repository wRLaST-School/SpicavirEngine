#include "GPipelineManager.h"
#include "SpShader.h"
#include "GPipeline.h"
#include "SpRootSignature.h"
#include "Model.h"
#include "Sprite.h"
#include <Particle.h>

void GPipelineManager::CreateAll()
{
#pragma region デフォルト3D
	RegisterShader("def");
	InitVS("def", "BasicVS.hlsl");
	InitGS("def", "BasicGS.hlsl");
	InitPS("def", "BasicPS.hlsl");

	PipelineDesc defDesc;
	defDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	defDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	defDesc.RootSignature.ptr = SpRootSignature::Get("3D")->rootsignature.Get();

	defDesc.Shader.pShader = GetShader("def");

	GPipeline::Create(defDesc, "def");
#pragma endregion

#pragma region パーティクル3D
	RegisterShader("particle");
	InitVS("particle", "ParticleVS.hlsl");
	InitGS("particle", "ParticleGS.hlsl");
	InitPS("particle", "ParticlePS.hlsl");

	PipelineDesc particleDesc;
	particleDesc.Render.InputLayout.pInputElementDescs = ParticleCommon::inputLayout;
	particleDesc.Render.InputLayout.NumElements = _countof(ParticleCommon::inputLayout);

	particleDesc.Render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	particleDesc.Blend.Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Add);

	particleDesc.RootSignature.ptr = SpRootSignature::Get("Particle")->rootsignature.Get();

	particleDesc.Shader.pShader = GetShader("particle");

	GPipeline::Create(particleDesc, "particle");
#pragma endregion

#pragma region デフォルト2D
	RegisterShader("2d");
	InitVS("2d", "SpriteVS.hlsl");
	InitPS("2d", "SpritePS.hlsl");

	PipelineDesc pl2dDesc;
	pl2dDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDesc.RootSignature.ptr = SpRootSignature::Get("2D")->rootsignature.Get();

	pl2dDesc.Shader.pShader = GetShader("2d");

	pl2dDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(pl2dDesc, "2d");
#pragma endregion

#pragma region 追加用（追加するときに下にコピペ）

#pragma endregion

}
