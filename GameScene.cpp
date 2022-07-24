#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"

void GameScene::Init()
{
	wTextureManager::Init();
	camera.SetRenderSize(GetwWindow()->width, GetwWindow()->height);
	camera.nearZ = 0.1f;
	camera.farZ = 1000.0f;
	camera.fov = PI / 2;

	topCamera.SetRenderSize(256, 256);
	topCamera.nearZ = 0.1f;
	topCamera.farZ = 1000.0f;
	topCamera.fov = PI / 2;

	model = Model("monkey");
	cubem = Model("cube");

	sky = Model("skydome");

	monkey.model = &model;

	skysphere.model = &sky;

	float boxsize = 2;
	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].model = &cubem;
			floor[i][j].position = { -boxsize * TileQuant/2 + i * boxsize, -3, -boxsize * TileQuant/2 + j * boxsize };
			floor[i][j].UpdateMatrix();
		}
	}

	texture = wTextureManager::LoadTexture("Resources/white.png", "white");
	wTextureManager::LoadTexture("Resources/think.png", "think");

	RTVManager::CreateRenderTargetTexture(256, 256, "offscreen");
	osrspr = Sprite("offscreen");
}

void GameScene::Update()
{
	camera.position = (Vec3)camera.position + Vec3((Key::Down(DIK_RIGHT) - Key::Down(DIK_LEFT)) * 0.25f, (Key::Down(DIK_SPACE) - Key::Down(DIK_LSHIFT)) * 0.25f, (Key::Down(DIK_UP) - Key::Down(DIK_DOWN)) * 0.25f);
	camera.rotation = (Vec3)camera.rotation + Vec3((Key::Down(DIK_NUMPAD2) - Key::Down(DIK_NUMPAD8)) * 0.05f, (Key::Down(DIK_NUMPAD6) - Key::Down(DIK_NUMPAD4)) * 0.05f, 0);
	camera.UpdateMatrix();

	switch (ctrlmode)
	{
	case ControllMode::BIOHAZERD:
	{
		if (Key::Triggered(DIK_Q)) ctrlmode = ControllMode::TPS;

		monkey.rotation = (Vec3)monkey.rotation + Vec3(0, PI / 144 * (Key::Down(DIK_D) - Key::Down(DIK_A)), 0);
		monkey.scale = (Vec3)monkey.scale + Vec3(Key::Down(DIK_M) - Key::Down(DIK_C), Key::Down(DIK_F) - Key::Down(DIK_V), 0);
		monkey.UpdateMatrix();
		Vec3 front = monkey.matWorld.ExtractAxisZ(monkey.scale.z);

		Vec3 move = front.SetLength(0.25f) * (Key::Down(DIK_W) - Key::Down(DIK_S));
		monkey.position = (Vec3)monkey.position + move;
		monkey.UpdateMatrix();
		break;
	}
	case ControllMode::TPS:
	{
		if (Key::Triggered(DIK_Q)) ctrlmode = ControllMode::BIOHAZERD;
		//monkey.rotation = (Vec3)monkey.rotation + Vec3(PI / 144 * (KeyDown(DIK_DOWN) - KeyDown(DIK_UP)), PI / 144 * (KeyDown(DIK_RIGHT) - KeyDown(DIK_LEFT)), 0);

		monkey.UpdateMatrix();
		Vec3 front = camera.matWorld.ExtractAxisZ();
		Vec3 right = camera.matWorld.ExtractAxisX();

		Vec3 move = front.GetNorm() * (Key::Down(DIK_W) - Key::Down(DIK_S));
		move += right.GetNorm() * (Key::Down(DIK_D) - Key::Down(DIK_A));
		if(move.GetSquaredLength() != 0)move.SetLength(0.25f);

		monkey.position = (Vec3)monkey.position + move;
		monkey.UpdateMatrix();
		break;
	}
	default:
		break;
	}

	topCamera.position.x = monkey.position.x;
	topCamera.position.y = 10;
	topCamera.position.z = monkey.position.z;
	topCamera.rotation = { PI / 2, 0.0f, 0.0f };
	topCamera.UpdateMatrix();

	hsv.x++;
	Float3 rgb = ConvertHSVtoRGB(hsv);
	color = {rgb.x, rgb.y, rgb.z, 255.0f};

	monkey.brightnessCB = {color.x / 255, color.y = 255, color.z / 255, color.w / 255};

	skysphere.UpdateMatrix();

	osrspr.position = Float3{ 128, 128, 0 };
	osrspr.scale = Float3{ 1.0f, 1.0f, 1.0f };
	osrspr.UpdateMatrix();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{	
	Camera::Set(camera);

	skysphere.Draw();
	monkey.Draw();

	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].Draw("think");
		}
	}

	RTVManager::SetRenderTargetToTexture("offscreen");
	Camera::Set(topCamera);
	skysphere.Draw();
	monkey.Draw();

	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].Draw("think");
		}
	}

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void GameScene::DrawSprite()
{
	Camera::Set(camera);
	osrspr.Draw();
}