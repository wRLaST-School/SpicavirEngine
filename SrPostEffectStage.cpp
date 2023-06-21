#include "stdafx.h"
#include "SrPostEffectStage.h"
#include <RTVManager.h>
#include <SpWindow.h>
#include <Bloom.h>
#include <SpImGui.h>

void SrPostEffectStage::Init()
{
	//int w = GetSpWindow()->width;
	//int h = GetSpWindow()->height;
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomBefore", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "BloomAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, 1.f, "Bloom2ndAfter", true);
	RTVManager::CreateRenderTargetTexture(.5f, .5f, "Bloom3rdAfter", true);
	SpTextureManager::LoadTexture("Resources/black.png", "black");
	SpTextureManager::AddMasterTextureKey("BloomBefore");
	SpTextureManager::AddMasterTextureKey("BloomAfter");
	SpTextureManager::AddMasterTextureKey("Bloom2ndAfter");
	SpTextureManager::AddMasterTextureKey("Bloom3rdAfter");

	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
}

void SrPostEffectStage::PreDraw(){};
void SrPostEffectStage::PostDraw(){};
void SrPostEffectStage::Render()
{
	//Object
	SpImGui::Command([&]() {
		ImGui::SetNextWindowPos(ImVec2(100, 420));
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		if (ImGui::Begin("PostEffects"))
		{
			if (ImGui::Button("UseBloom"))
			{
				useGauss = false;
				useBloom = true;
			}

			if (ImGui::Button("UseGaussian"))
			{
				useBloom = false;
				useGauss = true;
			}

			if (ImGui::Button("Turn Off Post Effects"))
			{
				useBloom = false;
				useGauss = false;
			}
		}
		ImGui::End();
	});

	if (useBloom)
	{
		BloomP1::Effect("BloomBefore", "BloomAfter");
		BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
		BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
		BloomFin::Effect("BloomBefore", "Bloom3rdAfter", "CurrentBuffer");
	}
	if (useGauss)
	{
		BloomP2::Effect("BloomBefore", "Bloom2ndAfter");
		BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
		BloomFin::Effect("Bloom3rdAfter", "black", "CurrentBuffer");
	}
	if (!useBloom && !useGauss)
	{
		BloomFin::Effect("BloomBefore", "black", "CurrentBuffer");
	}

}

void SrPostEffectStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
