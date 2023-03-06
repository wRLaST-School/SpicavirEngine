#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>
#include <Input.h>
#include <SpImGui.h>
#include <SpSwapChainManager.h>
#include <Bloom.h>
#include <SoundManager.h>

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

	pane.model = ModelManager::GetModel("SmoothSphere");
	pane2.model = ModelManager::GetModel("Cube");
	sky.model = ModelManager::GetModel("Sky");

	light1 = Light::GetPointLightPtr(Light::CreatePointLight({5.f, 1.f, -5.f}, {1.f, 1.f, 1.f}, {.05f, .0f, .02f}, "light1"));
	light2 = Light::GetPointLightPtr(Light::CreatePointLight({ -10.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, { .05f, .0f, .02f }, "light2"));
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

	if (Input::Key::Triggered(DIK_B))
	{
		SoundManager::Play("Laser");
	}

	//pane.scale = { .01f, .01f, .01f};
	sky.scale = { 5,5,5 };

	pane.rotation = (Vec3(0, 0.03f * (Input::Pad::Down(Button::Left) - Input::Pad::Down(Button::Right)), 0)) + pane.rotation;

	*pane2.brightnessCB.contents = Float4{ 0.f, 1.f, 1.f, .2f };
	pane2.UpdateMatrix();

	camera.UpdateMatrix();
	pane.UpdateMatrix();
	sky.UpdateMatrix();

	////style editor
	//SpImGui::Command([&]() { ImGui::ShowStyleEditor(); });

	//Object
	SpImGui::Command([&]() {
		ImGui::SetNextWindowPos(ImVec2(100, 220));
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		if (ImGui::Begin("Object Editor"))
		{
			ImGui::SliderFloat3("Position", &pane.position.x, -30.f, 30.f);
			ImGui::SliderFloat3("Scale", &pane.scale.x, -1.f, 1.f);
			ImGui::SliderFloat3("Rotation", &pane.rotation.x, 0.f, 2.f * PIf);

			if (ImGui::Button("UseSmooth"))
			{
				pane.model = ModelManager::GetModel("SmoothSphere");
			}

			if (ImGui::Button("UseFlat"))
			{
				pane.model = ModelManager::GetModel("FlatSphere");
			}
		}
	ImGui::End();
	});

	SpImGui::Command([&]() {
		ImGui::SetNextWindowPos(ImVec2(100, 160));
		ImGui::SetNextWindowSize(ImVec2(100, 60));
		if (ImGui::Begin("GamePad"))
		{
			ImGui::InputInt("Index", &Input::Pad::GetInstance()->gamepadIndex);
		}
		ImGui::End();
	});

	light1->DrawFrame();
	light2->DrawFrame();
	light1->DrawLightEditor();
	light2->DrawLightEditor();

	//anim.Update();
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);

	pane2.DrawAlpha("white");
	pane.Draw("white");
	sky.Draw();

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, GetSpWindow()->height / 2, 1, 1, 0, "particle1", Anchor::Center, Color(0xffffff));
}
