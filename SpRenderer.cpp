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

}

SpRenderer* SpRenderer::GetInstance()
{
	static SpRenderer rndr;
	return &rndr;
}
