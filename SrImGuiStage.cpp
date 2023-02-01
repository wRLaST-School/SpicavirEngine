#include "stdafx.h"
#include "SrImGuiStage.h"
#include <SpImGui.h>
void SrImGuiStage::Init()
{
}

void SrImGuiStage::PreDraw()
{
}

void SrImGuiStage::Render()
{
	SpImGui::Draw();
}

void SrImGuiStage::PostDraw()
{
}

void SrImGuiStage::DrawCommands(function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
