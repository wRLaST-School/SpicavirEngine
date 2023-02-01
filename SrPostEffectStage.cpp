#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>

void SrPostEffectStage::Init()
{
	int w = GetSpWindow()->width;
	int h = GetSpWindow()->height;
	RTVManager::CreateRenderTargetTexture(w, h, "BloomBefore");
	RTVManager::CreateRenderTargetTexture(w, h, "BloomAfter");
	RTVManager::CreateRenderTargetTexture(w / 2, h, "Bloom2ndAfter");
	RTVManager::CreateRenderTargetTexture(w / 2, h/2, "Bloom3rdAfter");
}
