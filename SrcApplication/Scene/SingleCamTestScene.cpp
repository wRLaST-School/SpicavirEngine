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
	SpTextureManager::LoadTexture("Resources/circleParticle.png", "particle1");
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();

	sphere.model = ModelManager::GetModel("SmoothSphere");
	floor.model = ModelManager::GetModel("Pane");

	sky.model = ModelManager::GetModel("Sky");
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

	floor.position = { 0.f, -0.75f, 0.f};
	floor.rotation.x = PIf / 2.f;
	sphere.position = {0.f, sinf((float)timer / 60), 0.f};
	spCol.pos = sphere.position;
	spCol.r = sphere.scale.x;
	plCol.norm = Vec3(0.f, 1.f, 0.f);
	plCol.distance = floor.position.y;

	ray.origin = Vec3(2.0f, 1.f, -sinf((float)timer / 60));
	ray.ray = Vec3(0, -1.f, 0);

	rCol.r = ray;

	*floor.brightnessCB.contents = {1.f, 1.f, 1.f, 0.35f};
	//“–‚½‚è”»’è
	if (spCol.Collide(plCol))
	{
		*sphere.brightnessCB.contents = { 1.f, 0.f, 0.f, 1.f };
	}
	else
	{
		*sphere.brightnessCB.contents = { 1.f, 1.f, 1.f, 1.f };
	}

	camera.UpdateMatrix();
	floor.UpdateMatrix();
	sphere.UpdateMatrix();
	sky.UpdateMatrix();

	timer++;
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);

	sphere.Draw("white");
	floor.Draw("white");
	sky.Draw();

	LineDrawer::DrawLine(ray.origin, ray.origin + ray.ray * 100, {1.f, 0.f, 0.f, 1.f});

	Vec3 hit;
	rCol.Collide(plCol, &hit);
	LineDrawer::DrawCube(hit, { 0.5f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f, 1.0f });

	LineDrawer::DrawAllLines();

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
