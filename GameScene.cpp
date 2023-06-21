#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>

void GameScene::LoadResources()
{
	Boss::Load();
	Player::Load();
	ModelManager::Register("Resources/Models/Floor.glb", "floor", true);
}

void GameScene::Init()
{
	Boss::Set(&boss);
	Player::Set(&player);

	boss.Init();
	player.Init();

	floor.model = ModelManager::GetModel("floor");
	floor.position = { 0.f, 0.f, 0.f };
	floor.scale = { 15.f, 1.f, 15.f };
	floor.UpdateMatrix();

	Light::sDirectional.direction = Vec3(1, -1, 0).GetNorm();

	cam.Init();

	CameraController::Set(&cam);
}

void GameScene::Update()
{
	boss.Update();
	player.Update();
	cam.Update();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	cam.Set();

	floor.Draw("white");
	boss.Draw();
	player.Draw();
}

void GameScene::DrawSprite()
{
}
