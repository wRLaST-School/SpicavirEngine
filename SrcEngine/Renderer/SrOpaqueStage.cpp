#include "stdafx.h"
#include "SrOpaqueStage.h"
#include <Light.h>
#include <Camera.h>
#include <RTVManager.h>
#include <LineDrawer.h>
void SrOpaqueStage::Init()
{
}

void SrOpaqueStage::PreDraw()
{
	SpDirectX* dx = GetSpDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("def"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = (FLOAT)GetSpWindow()->width;
	viewport.Height = (FLOAT)GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	dx->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // 切り抜き座標下

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	dx->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Light::Use();
	Camera::UseCurrent();

	RTVManager::SetRenderTargetToTexture("BloomBefore");

	//vector<TextureKey> rts = { "normalTest", "inverseTest" };
	//RTVManager::SetRenderTargets(rts);
}

void SrOpaqueStage::PostDraw()
{
}

void SrOpaqueStage::Render()
{
	for (auto& cmd : commands_)
	{
		cmd();
	}
	commands_.clear();

	LineDrawer::DrawAllLines();
}

void SrOpaqueStage::DrawCommands(std::function<void(void)> cmd)
{
	commands_.push_back(cmd);
}
