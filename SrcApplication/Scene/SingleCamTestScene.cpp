#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>
#include <Input.h>
#include <SpImGui.h>

void SingleCamTestScene::LoadResources()
{
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("square", "Pane");
	ModelManager::Register("skydome", "Sky");

	SpTextureManager::LoadTexture("Resources/white.png", "white");
	SpTextureManager::LoadTexture("Resources/circleParticle.png", "particle1");
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();

	pane.model = ModelManager::GetModel("Pane");
	sky.model = ModelManager::GetModel("Sky");

	e1.position.x = 6;
	e1.position.z = 3;
	e1.radius = {3.f, 3.f, 3.f};

	e2.position.x = -6;
	e2.position.z = 3;
	e2.radius = { 3.f, 3.f, 3.f };
	e2.shape = Emitter<ParticleSample2>::Shape::Sphere;
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

	pane.scale = { 0.02f, 0.02f, 0.02f};
	sky.scale = { 5,5,5 };

	camera.UpdateMatrix();
	pane.UpdateMatrix();
	sky.UpdateMatrix();

	SpImGui::Command([&]() { ImGui::ShowDemoWindow(); });
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);

	pane.Draw();
	sky.Draw();
	e1.Draw();
	e2.Draw();

	LineDrawer::DrawAllLines();
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
