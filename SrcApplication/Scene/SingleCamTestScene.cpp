#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>
#include <Input.h>
#include <SpImGui.h>
#include <SpSwapChainManager.h>
#include <Bloom.h>

void SingleCamTestScene::LoadResources()
{
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("20Surface", "20s");
	ModelManager::Register("square", "Pane");
	ModelManager::Register("skydome", "Sky");
	//ModelManager::Register("Resources/Models/Minion/Minion_FBX.fbx", "fbxtest", true);
	ModelManager::Register("Resources/Models/fbxtest/fbxtest.fbx", "fbxtest", true);
	ModelManager::Register("Resources/Models/fbxtest/fbxtest2.fbx", "fbxtest3", false);
	ModelManager::Register("ICO", "ssTest2");

	ModelManager::Register("Resources/Models/testOBJ/testFBX.fbx", "fbxtest2", false);
	ModelManager::Register("testOBJ", "ssTest");

	SpTextureManager::LoadTexture("Resources/white.png", "white");
	SpTextureManager::LoadTexture("Resources/black.png", "black");
	SpTextureManager::LoadTexture("Resources/circleParticle.png", "particle1");

	RTVManager::CreateRenderTargetTexture(1920, 1080, "BloomBefore");
	RTVManager::CreateRenderTargetTexture(1920, 1080, "BloomAfter");
	RTVManager::CreateRenderTargetTexture(960, 1080, "Bloom2ndAfter");
	RTVManager::CreateRenderTargetTexture(960, 540, "Bloom3rdAfter");
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();

	pane.model = ModelManager::GetModel("fbxtest");
	sky.model = ModelManager::GetModel("Sky");

	e1.position.x = 6;
	e1.position.z = 3;
	e1.radius = {3.f, 3.f, 3.f};

	e2.position.x = -6;
	e2.position.z = 3;
	e2.radius = { 3.f, 3.f, 3.f };
	e2.shape = Emitter<ParticleSample2>::Shape::Sphere;

	light1 = Light::GetPointLightPtr(Light::CreatePointLight({0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, {.1f, .1f, .1f}, "light1"));
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

	float spd = 0.8f;

	Vec3 move =
		front.SetLength((Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S) + (Input::Pad::GetLStick()).y / 1000) * spd) +
		right.SetLength((Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A) + (Input::Pad::GetLStick()).x / 1000) * spd) +
		up.SetLength((Input::Key::Down(DIK_SPACE) - Input::Key::Down(DIK_LSHIFT) + Input::Pad::Down(Button::L) - Input::Pad::Down(Trigger::Left)) * spd);

	camera.position = move + camera.position;

	float curr = e1.radius.x;
	curr = max(0, curr + (Input::Key::Down(DIK_V) - Input::Key::Down(DIK_C) + Input::Pad::Down(Button::Right) - Input::Pad::Down(Button::Left)) * 0.1f);
	
	e1.radius = { curr, curr, curr };
	e2.radius = { curr, curr, curr };
#pragma endregion

	e1.Update();
	e2.Update();

	if (Input::Key::Down(DIK_X) || Input::Pad::Down(Button::R))
	{
		e1.DrawEmitArea();
		e2.DrawEmitArea();
	}

	//pane.scale = { .01f, .01f, .01f};
	sky.scale = { 5,5,5 };

	pane.rotation = (Vec3(0, 0.03 * (Input::Pad::Down(Button::Left) - Input::Pad::Down(Button::Right)), 0)) + pane.rotation;

	camera.UpdateMatrix();
	pane.UpdateMatrix();
	sky.UpdateMatrix();

	//style editor
	SpImGui::Command([&]() { ImGui::ShowStyleEditor(); });

	//Object
	SpImGui::Command([&]() { 
		if (ImGui::Begin("Object Editor"))
		{
			ImGui::SliderFloat3("Position", &pane.position.x, -30.f, 30.f);
			ImGui::SliderFloat3("Scale", &pane.scale.x, -1.f, 1.f);
			ImGui::SliderFloat3("Rotation", &pane.rotation.x, 0.f, 2.f * PIf);

			if (ImGui::Button("UseSmooth"))
			{
				pane.model = ModelManager::GetModel("fbxtest");
			}

			if (ImGui::Button("UseFlat"))
			{
				pane.model = ModelManager::GetModel("fbxtest2");
			}

			if (ImGui::Button("UseObj"))
			{
				pane.model = ModelManager::GetModel("ssTest");
			}

			if (ImGui::Button("UseICO fbx"))
			{
				pane.model = ModelManager::GetModel("fbxtest3");
			}

			if (ImGui::Button("UseICO obj"))
			{
				pane.model = ModelManager::GetModel("ssTest2");
			}
			ImGui::End();
		}
	});

	//light
	SpImGui::Command([&]() { 
		if (ImGui::Begin("Light Editor"))
		{
			ImGui::SliderFloat3("Position", &light1->pos.x, -30.f, 30.f);
			ImGui::SliderFloat3("Color", &light1->color.x, 0.f, 3.f);
			ImGui::SliderFloat3("Attenuation", &light1->att.x, 0.f, 1.f);
			ImGui::Checkbox("Active", &light1->isActive);

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

	SpImGui::Command([&]() {
		if (ImGui::Begin("Post Effect"))
		{
			ImGui::Checkbox("Bloom", &enableBloom);
			ImGui::Checkbox("Gauss", &enableGauss);
			ImGui::End();
		}
	});
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);

	if (enableBloom || enableGauss)
	{
		RTVManager::SetRenderTargetToTexture("BloomBefore");
	}

	pane.Draw("white");
	sky.Draw();
	e1.Draw();
	e2.Draw();

	LineDrawer::DrawAllLines();

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());

	
	//BloomP1::Effect("BloomBefore", "CurrentBuffer");

	if (enableBloom)
	{
		BloomP1::Effect("BloomBefore", "BloomAfter");
		BloomP2::Effect("BloomAfter", "Bloom2ndAfter");
		BloomP3::Effect("Bloom2ndAfter", "Bloom3rdAfter");
		BloomFin::Effect("BloomBefore","Bloom3rdAfter", "CurrentBuffer");
	}

	if (enableGauss)
	{
		BloomP2::Effect("BloomBefore", "BloomAfter");
		BloomP3::Effect("BloomAfter", "Bloom2ndAfter");
		BloomFin::Effect("black", "Bloom2ndAfter", "CurrentBuffer");
	}
}

void SingleCamTestScene::DrawSprite()
{

	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
