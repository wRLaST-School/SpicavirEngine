#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>
#include <SpDS.h>
#include <LevelManager.h>
#include <SpEffekseer.h>

void GameScene::LoadResources()
{
	Boss::Load();
	Player::Load();
	ModelManager::Register("Resources/Models/Floor.glb", "floor", true);
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("triangle", "Triangle");
	SpTextureManager::LoadTexture("Resources/circleParticle.png", "BasicParticle");
	SpEffekseer::Load(L"Resources/Effekseer", L"Resources/Effekseer/Marker/Marker.efk", "Marker");
	SpEffekseer::Load(L"Resources/Effekseer/Line", L"Resources/Effekseer/Line/Line.efk", "LineAttack");
}

void GameScene::Init()
{
	LevelManager::Init();
	Boss::Set(&boss);
	Player::Set(&player);

	boss.Init();
	player.Init();

	Light::sDirectional.direction = Vec3(1, -1, 0).GetNorm();

	cam.Init();

	CameraController::Set(&cam);
}

void GameScene::Update()
{
	LevelManager::Update();
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

	LevelManager::Draw();
	boss.Draw();
	player.Draw();
}

void GameScene::DrawSprite()
{
	SpDS::DrawBox(100, 100, 150, 400, Color::Green);
}
