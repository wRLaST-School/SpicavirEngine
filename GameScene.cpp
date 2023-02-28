#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>

void GameScene::LoadResources()
{
	Player::Load();
}

void GameScene::Init()
{
	Player::Init();
}

void GameScene::Update()
{
	Player::Get()->Update();
}

void GameScene::DrawSprite()
{
	Player::Get()->Draw();
}
