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
			floor[i][j].posision = { -boxsize * TileQuant/2 + i * boxsize, -3, -boxsize * TileQuant/2 + j * boxsize };
			floor[i][j].UpdateMatrix();
		}
	}

	texture = wTextureManager::LoadTexture("Resources/white.png", "white");
	wTextureManager::LoadTexture("Resources/think.png", "think");
}

void GameScene::Update()
{
	camera.posision = (Vec3)camera.posision + Vec3((Key::Down(DIK_RIGHT) - Key::Down(DIK_LEFT)) * 0.25f, (Key::Down(DIK_SPACE) - Key::Down(DIK_LSHIFT)) * 0.25f, (Key::Down(DIK_UP) - Key::Down(DIK_DOWN)) * 0.25f);
	camera.rotation = (Vec3)camera.rotation + Vec3((Key::Down(DIK_NUMPAD2) - Key::Down(DIK_NUMPAD8)) * 0.05f, (Key::Down(DIK_NUMPAD6) - Key::Down(DIK_NUMPAD4)) * 0.05f, 0);
	camera.UpdateMatrix();

	switch (ctrlmode)
	{
	case ControllMode::BIOHAZERD:
	{
		if (Key::Triggered(DIK_Q)) ctrlmode = ControllMode::TPS;

		monkey.rotation = (Vec3)monkey.rotation + Vec3(0, PI / 144 * (Key::Down(DIK_D) - Key::Down(DIK_A)), 0);
		monkey.scale = (Vec3)monkey.scale + Vec3(Key::Down(DIK_M) - Key::Down(DIK_C), Key::Down(DIK_T) - Key::Down(DIK_V), 0);
		monkey.UpdateMatrix();
		Vec3 front = monkey.matWorld.ExtractAxisZ(monkey.scale.z);

		Vec3 move = front.SetLength(0.25f) * (Key::Down(DIK_W) - Key::Down(DIK_S));
		monkey.posision = (Vec3)monkey.posision + move;
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

		monkey.posision = (Vec3)monkey.posision + move;
		monkey.UpdateMatrix();
		break;
	}
	default:
		break;
	}

	skysphere.UpdateMatrix();
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
}

void GameScene::DrawSprite()
{
	//spr.Draw();	
}