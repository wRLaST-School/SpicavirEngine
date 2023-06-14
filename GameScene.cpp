#include "stdafx.h"
#include "GameScene.h"

void GameScene::LoadResources()
{
	ModelManager::Register("Resources/Models/Boss/boss.glb", "Boss", true);
	SpTextureManager::LoadTexture("Resources/white.png", "white");
}

void GameScene::Init()
{
	boss.model = ModelManager::GetModel("Boss");

	cam.UseDefaultParams();

	cam.position = { 0, 0, -10 };
}

void GameScene::Update()
{
	boss.UpdateMatrix();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	Camera::Set(cam);

	boss.Draw("white");
}

void GameScene::DrawSprite()
{
}
