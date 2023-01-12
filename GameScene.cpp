#include "GameScene.h"
#include <Bloom.h>
#include <Input.h>
#include <SpImGui.h>
#include <SceneManager.h>

void GameScene::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/white.png", "white");

	ModelManager::Register("20Surface", "20sf");
	ModelManager::Register("skydome", "Sky");
	ModelManager::Register("Boss", "Boss");
	ModelManager::Register("sphere", "BBullet");
	ModelManager::Register("Floor", "Floor");
	ModelManager::Register("cube", "Cube");
	SpTextureManager::LoadTexture("Resources/circleParticle.png", "particle1");

	SpTextureManager::AddMasterTextureKey("SpicavirSky.png");
	SpTextureManager::AddMasterTextureKey("triTex.png");

	SoundManager::LoadWave("Resources/Sounds/counterSuccess.wav", "counterSuccess");
	SoundManager::LoadWave("Resources/Sounds/enShot.wav", "enShot");
	SoundManager::LoadWave("Resources/Sounds/PlayerDamaged.wav", "PlayerDamaged");
	SoundManager::LoadWave("Resources/Sounds/TP.wav", "Dodge");

	RTVManager::CreateRenderTargetTexture(1920, 1080, "BloomBefore");
	RTVManager::CreateRenderTargetTexture(1920, 1080, "BloomAfter");
	RTVManager::CreateRenderTargetTexture(960, 1080, "Bloom2ndAfter");
	RTVManager::CreateRenderTargetTexture(960, 540, "Bloom3rdAfter");
}

void GameScene::Init()
{
	camera.UseDefaultParams();
	camera.parent = &player;
	camera.position.y = 1.0f;
	camera.position.z = -5.0f;

	camera.rotation.x = PI / 45;

	player.model = ModelManager::GetModel("20sf");
	sky.model = ModelManager::GetModel("Sky");
	boss.model = ModelManager::GetModel("Boss");
	floor.model = ModelManager::GetModel("Floor");
	hp.model = ModelManager::GetModel("Cube");
	bossHP.model = ModelManager::GetModel("Cube");

	Player::Set(&player);
}

void GameScene::Update()
{
	player.Update();
	hp.Update();
	boss.Update();
	bossHP.Update();

	sky.scale = { 5,5,5 };

	camera.UpdateMatrix();
	*floor.brightnessCB.contents = {.1f, .1f, .1f, 1.f};
	floor.UpdateMatrix();

#ifdef _DEBUG
	SpImGui::Command([&] {
		if (ImGui::Begin("Main"))
		{
			ImGui::DragFloat3("Camera", &camera.position.x);

			ImGui::End();
		}
		});

	SpImGui::Command([&]() {
		if (ImGui::Begin("GamePad"))
		{
			ImGui::InputInt("Index", &Input::Pad::GetInstance()->gamepadIndex);

			ImGui::End();
		}
	});
#endif // _DEBUG

	SceneManager::Transition();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	Camera::Set(camera);
	RTVManager::SetRenderTargetToTexture("BloomBefore");

	sky.Draw();
	floor.Draw("white");
	player.Draw();
	hp.Draw("white");
	bossHP.Draw("white");
	boss.Draw();

	LineDrawer::DrawAllLines();

	BloomP1::Effect("BloomBefore", "BloomAfter");
	BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
	BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
	BloomFin::Effect("BloomBefore", "Bloom3rdAfter", "CurrentBuffer");
}

void GameScene::DrawSprite()
{
}
