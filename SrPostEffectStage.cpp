#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>
#include <Bloom.h>
#include <Monotone.h>

void SrPostEffectStage::Init()
{
	int w = GetSpWindow()->width;
	int h = GetSpWindow()->height;
	RTVManager::CreateRenderTargetTexture(w, h, "BloomBefore");
	RTVManager::CreateRenderTargetTexture(w, h, "BloomAfter");
	RTVManager::CreateRenderTargetTexture(w / 2, h, "Bloom2ndAfter");
	RTVManager::CreateRenderTargetTexture(w / 2, h/2, "Bloom3rdAfter");
	RTVManager::CreateRenderTargetTexture(w, h, "Mono");
	SpTextureManager::AddMasterTextureKey("BloomBefore");
	SpTextureManager::AddMasterTextureKey("BloomAfter");
	SpTextureManager::AddMasterTextureKey("Bloom2ndAfter");
	SpTextureManager::AddMasterTextureKey("Bloom3rdAfter");
	SpTextureManager::AddMasterTextureKey("Mono");

	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
	Monotone::Init();
}

void SrPostEffectStage::PreDraw(){};
void SrPostEffectStage::PostDraw(){};
void SrPostEffectStage::Render()
{
	BloomP1::Effect("BloomBefore", "BloomAfter");
	BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
	BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
	BloomFin::Effect("BloomBefore", "Bloom3rdAfter", "CurrentBuffer");

	/*Monotone::Effect("BloomBefore", "CurrentBuffer");*/

}

void SrPostEffectStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
