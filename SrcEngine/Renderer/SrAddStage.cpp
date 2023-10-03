#include "stdafx.h"
#include "SrAddStage.h"
#include <Model.h>
#include <Camera.h>

void SrAddStage::Init()
{
#pragma region �f�t�H���g3D
	PipelineDesc defDesc;
	defDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	defDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	defDesc.RootSignature.ptr = SpRootSignature::Get("NoLight3D")->rootsignature.Get();

	defDesc.Shader.pShader = GetShader("lightless");
	defDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	defDesc.Blend[0].Desc = PipelineUtil::Blend::GetBlendMode(PipelineUtil::BlendMode::Add);
	defDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	GPipeline::Create(defDesc, "Add3D");
#pragma endregion
}

void SrAddStage::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("Add3D"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("NoLight3D")->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = (FLOAT)GetSpWindow()->width;
	viewport.Height = (FLOAT)GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	dx->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // �؂蔲�����W��
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // �؂蔲�����W�E
	scissorrect.top = 0;                                        // �؂蔲�����W��
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // �؂蔲�����W��

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	dx->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	Camera::UseCurrent();
}

void SrAddStage::PostDraw()
{
}

void SrAddStage::Render()
{
	for (auto& cmd : commands_)
	{
		cmd();
	}
	commands_.clear();
}

void SrAddStage::DrawCommands(std::function<void(void)> cmd)
{
	commands_.push_back(cmd);
}
