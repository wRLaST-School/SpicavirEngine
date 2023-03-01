#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>
#include <SpDS.h>
#include <Util.h>

void GameScene::LoadResources()
{
	Player::Load();
	SpTextureManager::LoadTexture("Resources/overlay.png", "overlay");
	SpTextureManager::LoadTexture("Resources/white.png", "white");
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
}

void GameScene::Update()
{
	Player::Get()->Update();
}

void GameScene::DrawSprite()
{
	//back
	SpDS::DrawBox(GetSpWindow()->width / 2, GetSpWindow()->height / 2, GetSpWindow()->width, GetSpWindow()->height, 0, Color(0x332288));

	int basebgpsize = 30;
	Vec2 win(GetSpWindow()->width, GetSpWindow()->height);
	for (int i = 0; i < maxbackp; i++)
	{
		Vec2& p = backpos[i];
		SpDS::DrawBox(p.x, p.y, basebgpsize * 1920 / win.x, basebgpsize * 1080 / win.y,0, backcolor[i]);
	}

	//main
	Player::Get()->Draw();
	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, GetSpWindow()->height / 2, 1920.f / GetSpWindow()->width, 1920.f / GetSpWindow()->width, 0, "overlay");
}
