#include "stdafx.h"
#include "SpRenderer.h"

void SpRenderer::Init()
{
	for (auto& stg : GetInstance()->stages)
	{
		stg->Init();
	}
}

void SpRenderer::Render()
{
	dynamic_cast<SrAlphaStage*>(GetInstance()->stages[(int)Stage::Alpha].get())->SortObjects();
	for (auto& stg : GetInstance()->stages)
	{
		stg->PreDraw();
		stg->Render();
		stg->PostDraw();
	}
}

SpRenderer* SpRenderer::GetInstance()
{
	static SpRenderer rndr;
	return &rndr;
}

void SpRenderer::DrawCommand(function<void(void)> cmd, Stage stg)
{
	GetInstance()->stages[(int)stg]->DrawCommands(cmd);
}

void SpRenderer::RegisterAlphaObj(Object3D* obj)
{
	dynamic_cast<SrAlphaStage*>(GetInstance()->stages[(int)Stage::Alpha].get())->RegisterAlphaObject(obj);
}
