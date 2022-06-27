#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"

void GameScene::Init()
{
	wTextureManager::Init();

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

	RTVManager::CreateRenderTargetTexture(720, 720, narrow);
	spr = Sprite(narrow);

	RokugaAruaru.model = &cubem;
	RokugaAruaru.posision = { 0,0,50 };
	RokugaAruaru.scale = {10, 10, 10};
	RokugaAruaru.UpdateMatrix();
}

void GameScene::Update()
{
	camera.posision = (Vec3)camera.posision + Vec3((KeyDown(DIK_RIGHT) - KeyDown(DIK_LEFT)) * 0.25f, (KeyDown(DIK_SPACE) - KeyDown(DIK_LSHIFT)) * 0.25f, (KeyDown(DIK_UP) - KeyDown(DIK_DOWN)) * 0.25f);
	camera.rotation = (Vec3)camera.rotation + Vec3((KeyDown(DIK_NUMPAD2) - KeyDown(DIK_NUMPAD8)) * 0.05f, (KeyDown(DIK_NUMPAD6) - KeyDown(DIK_NUMPAD4)) * 0.05f, 0);
	camera.UpdateMatrix();

	switch (ctrlmode)
	{
	case ControllMode::BIOHAZERD:
	{
		if (KeyTriggered(DIK_Q)) ctrlmode = ControllMode::TPS;

		monkey.rotation = (Vec3)monkey.rotation + Vec3(0, PI / 144 * (KeyDown(DIK_D) - KeyDown(DIK_A)), 0);

		monkey.UpdateMatrix();
		Vec3 front = monkey.matWorld.ExtractAxisZ();

		Vec3 move = front.SetLength(0.25f) * (KeyDown(DIK_W) - KeyDown(DIK_S));
		monkey.posision = (Vec3)monkey.posision + move;
		monkey.UpdateMatrix();
		break;
	}
	case ControllMode::TPS:
	{
		if (KeyTriggered(DIK_Q)) ctrlmode = ControllMode::BIOHAZERD;

		//monkey.rotation = (Vec3)monkey.rotation + Vec3(PI / 144 * (KeyDown(DIK_DOWN) - KeyDown(DIK_UP)), PI / 144 * (KeyDown(DIK_RIGHT) - KeyDown(DIK_LEFT)), 0);

		monkey.UpdateMatrix();
		Vec3 front = camera.matWorld.ExtractAxisZ();
		Vec3 right = camera.matWorld.ExtractAxisX();

		Vec3 move = front.GetNorm() * (KeyDown(DIK_W) - KeyDown(DIK_S));
		move += right.GetNorm() * (KeyDown(DIK_D) - KeyDown(DIK_A));
		if(move.GetSquaredLength() != 0)move.SetLength(0.25f);

		monkey.posision = (Vec3)monkey.posision + move;
		monkey.UpdateMatrix();
		break;
	}
	default:
		break;
	}


	Matrix pMat = Matrix::Projection(
		PI / 4,
		(float)GetwWindow()->width / GetwWindow()->height,
		0.1f, 1000.0f
	);

	Float3 eyeV = camera.matWorld.ExtractAxisZ();
	Float3 eye = camera.posision;
	Float3 up = (Vec3)camera.matWorld.ExtractAxisY();

	Matrix vMat = Matrix::ViewLookTo(eye, eyeV, up);
	vproj = vMat * pMat;

	spr.position = { 1280/2, 720/2, 0 };
	spr.UpdateMatrix();
	skysphere.UpdateMatrix();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{

	RTVManager::SetRenderTargetToTexture(narrow);
	skysphere.Draw(vproj);
	monkey.Draw(vproj);

	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].Draw(vproj, "think");
		}
	}
	RokugaAruaru.Draw(vproj, narrow);

	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
	RTVManager::ClearCurrentRenderTarget({0.0f, 0.0f, 0.0f, 0.0f});


	skysphere.Draw(vproj);
	monkey.Draw(vproj);

	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].Draw(vproj, "think");
		}
	}
	RokugaAruaru.Draw(vproj, narrow);
}

void GameScene::DrawSprite()
{
	spr.Draw();
}
