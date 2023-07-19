#include "stdafx.h"
#include "SpRenderer.h"

void SpRenderer::Init()
{
	for (auto& stg : GetInstance()->stages_)
	{
		stg->Init();
	}
}

void SpRenderer::Render()
{
	dynamic_cast<SrAlphaStage*>(GetInstance()->stages_[(int32_t)Stage::Alpha].get())->SortObjects();
	for (auto& stg : GetInstance()->stages_)
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

void SpRenderer::DrawCommand(std::function<void(void)> cmd, const Stage& stg)
{
	GetInstance()->stages_[(int32_t)stg]->DrawCommands(cmd);
}

void SpRenderer::RegisterAlphaObj(Object3D* obj)
{
	dynamic_cast<SrAlphaStage*>(GetInstance()->stages_[(int32_t)Stage::Alpha].get())->RegisterAlphaObject(obj);
}
