#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>
#include <SpDS.h>
#include <Util.h>
#include <Monotone.h>

void GameScene::LoadResources()
{
	Player::Load();
	SpTextureManager::LoadTexture("Resources/overlay.png", "overlay");
	SpTextureManager::LoadTexture("Resources/white.png", "white");
	SpTextureManager::LoadTexture("Resources/think.png", "think");
	SpTextureManager::LoadTexture("Resources/circleLight.png", "circle");
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
}

void GameScene::DrawSprite()
{
	back.Draw();

	//main
	Player::Get()->Draw();
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (mapchip[i][j])
			{
				SpDS::DrawBox(30 + j * mapsize, 30 + i * mapsize, mapsize, mapsize, 0, 0x00ff44);
			}
		}
	}
	SpDS::SetBlendMode(SpDS::Blend::Sub);

	//‰~Œ`”ÍˆÍŠO‚Ì‚Ýƒ‚ƒmƒg[ƒ“‚É
	SpDS::SetPreDrawFunc([] {Monotone::Effect("BloomBefore", "Mono"); });
	SpDS::SetRenderTarget("Mono");
	SpDS::SetBlendMode(SpDS::Blend::Sub);
	SpDS::DrawRotaGraph(Player::Get()->pos.x, Player::Get()->pos.y, 1.f, 1.f, 0.f, "circle");

	SpDS::SetRenderTarget("BloomBefore");
	SpDS::SetBlendMode(SpDS::Blend::Alpha);
	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, GetSpWindow()->height / 2, 1.f, 1.f, 0.f, "Mono");
}
