#include "stdafx.h"
#include "GameScene.h"
#include <Player.h>
#include <SpDS.h>
#include <LevelManager.h>
#include <SpEffekseer.h>
#include <SoundManager.h>
#include <MainTimer.h>
#include <GameManager.h>
#include <SpImGui.h>

void GameScene::LoadResources()
{
	Boss::Load();
	Player::Load();
	ModelManager::Register("Resources/Models/Floor.glb", "floor", true);
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("triangle", "Triangle");
	ModelManager::Register("skydome", "Sky");
	ModelManager::Register("Sphere", "Sphere");

	SpTextureManager::LoadTexture("Resources/circleParticle.png", "BasicParticle");
	SpTextureManager::LoadTexture("Resources/black.png", "black");
	SpTextureManager::LoadTexture("Resources/hexagonPattern.jpg", "hexagon");
	SpEffekseer::Load(L"Resources/Effekseer", L"Resources/Effekseer/Marker/Marker.efk", "Marker");
	SpEffekseer::Load(L"Resources/Effekseer/Line", L"Resources/Effekseer/Line/Line.efk", "LineAttack");
	SpEffekseer::Load(L"Resources/Effekseer/Sphere", L"Resources/Effekseer/Sphere/sphere.efk", "SphereParticle");

	SoundManager::LoadWave("Resources/Sounds/counterSuccess.wav", "counterSuccess");
	SoundManager::LoadWave("Resources/Sounds/dodge.wav", "dodge");
	SoundManager::LoadWave("Resources/Sounds/LineAttack.wav", "LineAttack");
	SoundManager::LoadWave("Resources/Sounds/marker.wav", "marker");
	SoundManager::LoadWave("Resources/Sounds/Slash3.wav", "Slash3");
	SoundManager::LoadWave("Resources/Sounds/Slash12.wav", "Slash12");
	SoundManager::LoadWave("Resources/Sounds/takeDamage.wav", "takeDamage");
	SoundManager::LoadWave("Resources/Sounds/RushImpact.wav", "RushImpact");

	MainTimer::Load();
}

void GameScene::Init()
{
	LevelManager::Init();
	Boss::Set(&boss_);
	Player::Set(&player_);

	boss_.Init();
	player_.Init();

	Light::sDirectional.direction = Vec3(1, -1, 0).GetNorm();

	cam_.Init();

	CameraController::Set(&cam_);

	MainTimer::Init();
}

void GameScene::Update()
{
	LevelManager::Update();
	boss_.Update();
	player_.Update();
	cam_.Update();

	MainTimer::Update();

	if (GameManager::sShowDebug)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Game Scene"))
			{
				ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
				ImGui::Checkbox("Show Hit Box", &GameManager::sShowHitBoxes);
			}
			ImGui::End();
		});
	}
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	cam_.Set();

	LevelManager::Draw();
	boss_.Draw();
	player_.Draw();

	MainTimer::Draw();
}

void GameScene::DrawSprite()
{
}
