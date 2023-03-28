#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>
#include <SpDS.h>
#include <Util.h>
#include <Monotone.h>
#include <SpImGui.h>

void GameScene::LoadResources()
{
	Player::Load();
	SpTextureManager::LoadTexture("Resources/overlay.png", "overlay");
	SpTextureManager::LoadTexture("Resources/white.png", "white");
	SpTextureManager::LoadTexture("Resources/think.png", "think");
	SpTextureManager::LoadTexture("Resources/circleLight.png", "circle");
	SpTextureManager::LoadTexture("Resources/Sample.png", "sample");
	back.Load();
}

void GameScene::Init()
{
	for (auto& p : backpos)
	{
		p = Vec2(Util::RNG(0, GetSpWindow()->width), Util::RNG(0, GetSpWindow()->height));
	}

	for (auto& c : backcolor)
	{
		c = Color(Util::RNG(0, 255), Util::RNG(0, 255), Util::RNG(0, 255));
	}

	Player::Init();
	back.Init();
}

void GameScene::Update()
{
	back.Update();
	Player::Get()->Update();

	SpImGui::Command([&]{
		if (ImGui::Begin("Sample"))
		{
			ImGui::Checkbox("Use Sample", &useSample);
		}
		ImGui::End();
	});
}

void GameScene::DrawSprite()
{
	back.Draw();

	//main
	if (useSample)
	{
		SpDS::DrawRotaGraph(0, 0, 2.f, 2.f, 0.f, "sample", Anchor::TopLeft);
	}
	Player::Get()->Draw();
	if (!useSample) {
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				if (mapchip[i][j])
				{
					SpDS::DrawBox(30 + j * mapsize, 30 + i * mapsize, mapsize, mapsize, 0, Color(0x864a2b));
				}
			}
		}
	}

	SpDS::DrawBoxLine(300, 300, 100, 100, Color(0xffc7f5), 4);

	//�~�`�͈͊O�̂݃��m�g�[����
	SpDS::SetBlendMode(SpDS::Blend::Sub);

	SpDS::SetPreDrawFunc([] {Monotone::Effect("BloomBefore", "Mono"); });
	SpDS::SetRenderTarget("Mono");
	SpDS::SetBlendMode(SpDS::Blend::Sub);
	SpDS::DrawRotaGraph(Player::Get()->pos.x, Player::Get()->pos.y, 1.f, 1.f, 0.f, "circle");

	SpDS::SetRenderTarget("BloomBefore");
	SpDS::SetBlendMode(SpDS::Blend::Alpha);
	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, GetSpWindow()->height / 2, 1.f, 1.f, 0.f, "Mono");
}
