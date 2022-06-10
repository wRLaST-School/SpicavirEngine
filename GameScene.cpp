#include "GameScene.h"
#include "Input.h"

void GameScene::Init()
{
	wTextureManager::Init();

	monkey.model = &model;

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

		float cameraDistance = 5.0f;

		Vec3 back = monkey.matWorld.ExtractAxisZ() * -1;

		back.SetLength(cameraDistance);

		Matrix pMat = Matrix::Projection(
			PI / 4,
			(float)GetwWindow()->width / GetwWindow()->height,
			0.1f, 1000.0f
		);

		Float3 target = monkey.posision;
		Float3 eye = (Vec3)monkey.posision + back;
		Float3 up{ 0, 1, 0 };

		Matrix vMat = Matrix::ViewLookAt(eye, target, up);
		vproj = vMat * pMat;
		break;
	}
	case ControllMode::TPS:
	{
		if (KeyTriggered(DIK_Q)) ctrlmode = ControllMode::BIOHAZERD;

		monkey.rotation = (Vec3)monkey.rotation + Vec3(PI / 144 * (KeyDown(DIK_DOWN) - KeyDown(DIK_UP)), PI / 144 * (KeyDown(DIK_RIGHT) - KeyDown(DIK_LEFT)), 0);

		monkey.UpdateMatrix();
		Vec3 front = monkey.matWorld.ExtractAxisZ();
		Vec3 right = monkey.matWorld.ExtractAxisX();

		Vec3 move = front.GetNorm() * (KeyDown(DIK_W) - KeyDown(DIK_S));
		move += right.GetNorm() * (KeyDown(DIK_D) - KeyDown(DIK_A));
		if(move.GetSquaredLength() != 0)move.SetLength(0.25f);

		monkey.posision = (Vec3)monkey.posision + move;
		monkey.UpdateMatrix();

		float cameraDistance = 5.0f;

		Vec3 back = monkey.matWorld.ExtractAxisZ() * -1;

		back.SetLength(cameraDistance);

		Matrix pMat = Matrix::Projection(
			PI / 4,
			(float)GetwWindow()->width / GetwWindow()->height,
			0.1f, 1000.0f
		);

		Float3 target = monkey.posision;
		Float3 eye = (Vec3)monkey.posision + back;
		Float3 up{ 0, 1, 0 };

		Matrix vMat = Matrix::ViewLookAt(eye, target, up);
		vproj = vMat * pMat;
		break;
	}
	default:
		break;
	}
	
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{	
	monkey.Draw(vproj, "white");

	for (size_t i = 0; i < TileQuant; i++)
	{
		for (size_t j = 0; j < TileQuant; j++)
		{
			floor[i][j].Draw(vproj, "think");
		}
	}
}

void GameScene::DrawSprite()
{
}
