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
	ModelManager::Register("Assets/Models/Floor.glb", "floor", true);
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("triangle", "Triangle");
	ModelManager::Register("skydome", "Sky");
	ModelManager::Register("Sphere", "Sphere");

	SpTextureManager::LoadTexture("Assets/Images/circleParticle.png", "BasicParticle");
	SpTextureManager::LoadTexture("Assets/Images/black.png", "black");
	SpTextureManager::LoadTexture("Assets/Images/hexagonPattern.jpg", "hexagon");
	SpEffekseer::Load(L"Assets/Effekseer", L"Assets/Effekseer/Marker/Marker.efk", "Marker");
	SpEffekseer::Load(L"Assets/Effekseer/Line", L"Assets/Effekseer/Line/Line.efk", "LineAttack");
	SpEffekseer::Load(L"Assets/Effekseer/Sphere", L"Assets/Effekseer/Sphere/sphere.efk", "SphereParticle");

	SoundManager::LoadWave("Assets/Sounds/counterSuccess.wav", "counterSuccess");
	SoundManager::LoadWave("Assets/Sounds/dodge.wav", "dodge");
	SoundManager::LoadWave("Assets/Sounds/LineAttack.wav", "LineAttack");
	SoundManager::LoadWave("Assets/Sounds/marker.wav", "marker");
	SoundManager::LoadWave("Assets/Sounds/Slash3.wav", "Slash3");
	SoundManager::LoadWave("Assets/Sounds/Slash12.wav", "Slash12");
	SoundManager::LoadWave("Assets/Sounds/takeDamage.wav", "takeDamage");
	SoundManager::LoadWave("Assets/Sounds/RushImpact.wav", "RushImpact");

	MainTimer::Load();
}

void GameScene::Init()
{
	AddComponent<Player>("Player");
	AddComponent<Boss>("Boss");

	player_ = GetComponent<Player>("Player");
	boss_ = GetComponent<Boss>("Boss");

	AddComponent<LevelManager>("Level Manager");
	Boss::Set(boss_);
	Player::Set(player_);

	boss_->Init();
	player_->Init();

	Light::sDirectional.direction = Vec3(1, -1, 0).GetNorm();

	cam_.Init();

	CameraController::Set(&cam_);

	MainTimer::Init();
}

void GameScene::Update()
{
	cam_.Update();

	MainTimer::Update();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	cam_.Set();

	MainTimer::Draw();
}

void GameScene::DrawSprite()
{
	if (GameManager::sShowDebug)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Game Scene"))
			{
				ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
				ImGui::Checkbox("Show Hit Box", &GameManager::sShowHitBoxes);
				ImGui::Checkbox("Debug Immunity", &GameManager::sDebugImmunity);
				ImGui::Checkbox("Pause Game", &GameManager::sDebugTimeStop);
			}
			ImGui::End();
			});
	}
}
