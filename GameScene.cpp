#include "GameScene.h"
#include <Bloom.h>
#include <Input.h>
#include <SpImGui.h>

void GameScene::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/white.png", "white");

	ModelManager::Register("20Surface", "20sf");
	ModelManager::Register("skydome", "Sky");
	ModelManager::Register("Boss", "Boss");
	ModelManager::Register("sphere", "BBullet");
	ModelManager::Register("Floor", "Floor");

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

	Player::Set(&player);
}

void GameScene::Update()
{
	player.Update();
	boss.Update();

	sky.scale = { 5,5,5 };

	camera.UpdateMatrix();
	*floor.brightnessCB.contents = {.1f, .1f, .1f, 1.f};
	floor.UpdateMatrix();

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
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	Camera::Set(camera);
	RTVManager::SetRenderTargetToTexture("BloomBefore");

	sky.Draw();
	player.Draw();
	boss.Draw();
	floor.Draw("white");

	LineDrawer::DrawAllLines();

	BloomP1::Effect("BloomBefore", "BloomAfter");
	BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
	BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
	BloomFin::Effect("BloomBefore", "Bloom3rdAfter", "CurrentBuffer");
}

void GameScene::DrawSprite()
{
}
