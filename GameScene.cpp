#include "stdafx.h"
#include "GameScene.h"

void GameScene::LoadResources()
{
	Boss::Load();
}

void GameScene::Init()
{
	Boss::Set(&boss);
	boss.Init();

	cam.UseDefaultParams();

	cam.position = { 0, 0, -10 };
}

void GameScene::Update()
{
	boss.Update();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	Camera::Set(cam);

	boss.Draw();
}

void GameScene::DrawSprite()
{
}
