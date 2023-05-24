#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>
#include <Input.h>
#include <SpImGui.h>
#include <SpSwapChainManager.h>
#include <Bloom.h>
#include <SoundManager.h>
#include <LevelManager.h>

void SingleCamTestScene::LoadResources()
{
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("20Surface", "20s");
	ModelManager::Register("square", "Pane");
	ModelManager::Register("skydome", "Sky");
	//ModelManager::Register("Resources/Models/Minion/Minion_FBX.fbx", "fbxtest", true);
	ModelManager::Register("Resources/Models/SmoothSphere/SmoothSphere.fbx", "SmoothSphere", true);
	ModelManager::Register("ICO", "FlatSphere");

	SpTextureManager::LoadTexture("Resources/white.png", "white");
	SpTextureManager::LoadTexture("Resources/black.png", "black");
	SpTextureManager::LoadSingleDiv("Resources/circleParticle.png", 100, 100, 100, 200, "particle1");

	SoundManager::LoadWave("Resources/Sounds/Laser.wav", "Laser");

	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "normalTest", true);
	RTVManager::CreateRenderTargetTexture(1.f, 1.f, "inverseTest", true);

	/*vector<TextureKey> boss3Keys{
		"boss1",
		"boss2",
		"boss3",
		"boss4",
		"boss5",
		"boss6"
	};

	SpTextureManager::LoadDiv("Resources/boss3_rest.png", 300, 350, 6, 1, boss3Keys);

	anim.Register(boss3Keys, 3, true, "def");*/
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();

	sky.model = ModelManager::GetModel("Sky");

	LevelManager::Init();
}

void SingleCamTestScene::Update()
{
#pragma region Camera
	//rot
	camera.rotation.x += (Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP) - (Input::Pad::GetRStick().y / 1000)) * PIf / 120;
	camera.rotation.y += (Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT) + (Input::Pad::GetRStick().x / 1000)) * PIf / 120;

	//move
	Matrix cm = Matrix::RotRollPitchYaw(camera.rotation);
	Vec3 front = cm.ExtractAxisZ();
	Vec3 right = cm.ExtractAxisX();
	Vec3 up = cm.ExtractAxisY();

	float spd = 0.2f;

	Vec3 move =
		front.SetLength((Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S) + (Input::Pad::GetLStick()).y / 1000) * spd) +
		right.SetLength((Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A) + (Input::Pad::GetLStick()).x / 1000) * spd) +
		up.SetLength((Input::Key::Down(DIK_SPACE) - Input::Key::Down(DIK_LSHIFT) + Input::Pad::Down(Button::L) - Input::Pad::Down(Trigger::Left)) * spd);

	camera.position = move + camera.position;
#pragma endregion

	sky.scale = { 5,5,5 };

	camera.UpdateMatrix();
	sky.UpdateMatrix();



	SpImGui::Command([&]() {
		ImGui::SetNextWindowPos(ImVec2(100, 160));
		ImGui::SetNextWindowSize(ImVec2(100, 60));
		if (ImGui::Begin("GamePad"))
		{
			ImGui::InputInt("Index", &Input::Pad::GetInstance()->gamepadIndex);
		}
		ImGui::End();
	});

	//anim.Update();

	timer++;
	LevelManager::Update();
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);
	sky.Draw();
	LevelManager::Draw();

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
	//SpDS::DrawLine(1000, 500, (int)Input::Mouse::GetPos().x, (int)Input::Mouse::GetPos().y, Color(0xffffff), 5);
	//SpDS::DrawRotaGraph(GetSpWindow()->width / 2, GetSpWindow()->height / 2, 1, 1, (float)timer * PIf / 180 * 10, "particle1", Anchor::TopLeft, Color(0xffffff));
	//SpDS::DrawRotaGraph(0, 0, 0.5f, 0.5f, 0, "normalTest", Anchor::TopLeft);
	//SpDS::DrawRotaGraph(1920, 1080, 0.5f, 0.5f, 0, "inverseTest", Anchor::BottomRight);
}
