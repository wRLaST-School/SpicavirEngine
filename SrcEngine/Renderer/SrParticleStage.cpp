#include "stdafx.h"
#include "SrParticleStage.h"
#include <Camera.h>
#include <SpEffekseer.h>

void SrParticleStage::Init()
{
}

void SrParticleStage::PreDraw()
{
	SpDirectX* dx = GetWDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("particle"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("Particle")->rootsignature.Get());

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

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	Camera::UseCurrent();
}

void SrParticleStage::PostDraw()
{
	SpEffekseer::Draw();
}

void SrParticleStage::Render()
{
	for (auto& cmd : commands_)
	{
		cmd();
	}
	commands_.clear();
}

void SrParticleStage::DrawCommands(std::function<void(void)> cmd)
{
	commands_.push_back(cmd);
}