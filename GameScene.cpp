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

	spr = Sprite("think");

	//FOVEATED RENDERING PARAS
	center.CreateTextureAndInit(PI / 12, 96, 96, 96, 96, "centerFove");
	middle.CreateTextureAndInit(PI / 6, 96, 96, 192, 192, "middleFove");
	outer.CreateTextureAndInit(PI / 2, 256, 144, 1280, 720, "outerFove");

	for (int eye = 0; eye < 2; eye++)
	{
		eyeRes[eye].center.CreateTextureAndInit(PI / 6, 96, 96, 192, 192, "centerFoveEye" + eye);
		eyeRes[eye].middle.CreateTextureAndInit(PI / 3, 208, 208, 416, 416, "middleFoveEye" + eye);
		eyeRes[eye].outer.CreateTextureAndInit(PI / 2, 128, 144, 640, 720, "outerFoveEye" + eye);
	}

	eyeRes[0].center.sprite.position = (Vec3)eyeRes[0].center.sprite.position - Vec3(1280/4,0,0);
	eyeRes[0].middle.sprite.position = (Vec3)eyeRes[0].middle.sprite.position - Vec3(1280 / 4, 0, 0);
	eyeRes[0].outer .sprite.position = (Vec3)eyeRes[0].outer .sprite.position - Vec3(1280 / 4, 0, 0);
	eyeRes[1].center.sprite.position = (Vec3)eyeRes[1].center.sprite.position + Vec3(1280 / 4, 0, 0);
	eyeRes[1].middle.sprite.position = (Vec3)eyeRes[1].middle.sprite.position + Vec3(1280 / 4, 0, 0);
	eyeRes[1].outer .sprite.position = (Vec3)eyeRes[1].outer .sprite.position + Vec3(1280 / 4, 0, 0);
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

	if (KeyTriggered(DIK_F)) { 
		renderMode = renderMode == RenderingMode::normal ? RenderingMode::foveated : renderMode == RenderingMode::foveated ? RenderingMode::vr : RenderingMode::normal; 
	}


	Matrix pMat = Matrix::Projection(
		PI / 2,
		(float)GetwWindow()->width / GetwWindow()->height,
		0.1f, 1000.0f
	);

	Float3 eyeV = camera.matWorld.ExtractAxisZ();
	Float3 eye = camera.posision;
	Float3 up = (Vec3)camera.matWorld.ExtractAxisY();

	vMat= Matrix::ViewLookTo(eye, eyeV, up);
	vproj = vMat * pMat;

	center.sprite.UpdateMatrix();
	middle.sprite.UpdateMatrix();
	outer.sprite.UpdateMatrix();

	for (int eye = 0; eye < 2; eye++)
	{
		eyeRes[eye].center.sprite.UpdateMatrix();
		eyeRes[eye].middle.sprite.UpdateMatrix();
		eyeRes[eye].outer.sprite.UpdateMatrix();
	}

	skysphere.UpdateMatrix();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{	
	switch (renderMode)
	{
	case RenderingMode::foveated:
	{
		center.SetViewportAndScissorsRect();
		RTVManager::SetRenderTargetToTexture(center.renderTex);
		center.SetViewProjectionMatrix(vMat);

		vproj = vMat * center.projection;

		skysphere.Draw();
		monkey.Draw();

		for (size_t i = 0; i < TileQuant; i++)
		{
			for (size_t j = 0; j < TileQuant; j++)
			{
				floor[i][j].Draw("think");
			}
		}

		middle.SetViewportAndScissorsRect();
		RTVManager::SetRenderTargetToTexture(middle.renderTex);
		middle.SetViewProjectionMatrix(vMat);

		skysphere.Draw();
		monkey.Draw();

		for (size_t i = 0; i < TileQuant; i++)
		{
			for (size_t j = 0; j < TileQuant; j++)
			{
				floor[i][j].Draw("think");
			}
		}

		outer.SetViewportAndScissorsRect();
		RTVManager::SetRenderTargetToTexture(outer.renderTex);
		outer.SetViewProjectionMatrix(vMat);

		skysphere.Draw();
		monkey.Draw();

		for (size_t i = 0; i < TileQuant; i++)
		{
			for (size_t j = 0; j < TileQuant; j++)
			{
				floor[i][j].Draw("think");
			}
		}

		//VPortとscissorrect,RTを戻す
		//TODO:関数化
		D3D12_VIEWPORT viewport{};

		viewport.Width = GetwWindow()->width;
		viewport.Height = GetwWindow()->height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		GetWDX()->cmdList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorrect{};

		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + GetwWindow()->width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // 切り抜き座標下

		GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

		RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
		break;
	}

	case RenderingMode::normal:
	{
		nrdata.SetDefaultVProjMat(vproj);
		nrdata.SetVProjMatToDefault();
		skysphere.Draw();
		monkey.Draw();

		for (size_t i = 0; i < TileQuant; i++)
		{
			for (size_t j = 0; j < TileQuant; j++)
			{
				floor[i][j].Draw("think");
			}
		}
		break;
	}

	case RenderingMode::vr:
	{
		for (int eye = 0; eye < 2; eye++) {
			Float3 eyeV = camera.matWorld.ExtractAxisZ();
			Float3 eyePos = (Vec3)camera.posision - camera.matWorld.ExtractAxisX().SetLength(0.5f) + camera.matWorld.ExtractAxisX().SetLength(1 * eye);
			Float3 up = (Vec3)camera.matWorld.ExtractAxisY();

			vMat = Matrix::ViewLookTo(eyePos, eyeV, up);

			eyeRes[eye].center.SetViewportAndScissorsRect();
			RTVManager::SetRenderTargetToTexture(eyeRes[eye].center.renderTex);
			eyeRes[eye].center.SetViewProjectionMatrix(vMat);

			vproj = vMat * eyeRes[eye].center.projection;

			skysphere.Draw();
			monkey.Draw();

			for (size_t i = 0; i < TileQuant; i++)
			{
				for (size_t j = 0; j < TileQuant; j++)
				{
					floor[i][j].Draw("think");
				}
			}

			eyeRes[eye].middle.SetViewportAndScissorsRect();
			RTVManager::SetRenderTargetToTexture(eyeRes[eye].middle.renderTex);
			eyeRes[eye].middle.SetViewProjectionMatrix(vMat);

			skysphere.Draw();
			monkey.Draw();

			for (size_t i = 0; i < TileQuant; i++)
			{
				for (size_t j = 0; j < TileQuant; j++)
				{
					floor[i][j].Draw("think");
				}
			}

			eyeRes[eye].outer.SetViewportAndScissorsRect();
			RTVManager::SetRenderTargetToTexture(eyeRes[eye].outer.renderTex);
			eyeRes[eye].outer.SetViewProjectionMatrix(vMat);

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

		//VPortとscissorrect,RTを戻す
		//TODO:関数化
		D3D12_VIEWPORT viewport{};

		viewport.Width = GetwWindow()->width;
		viewport.Height = GetwWindow()->height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		GetWDX()->cmdList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorrect{};

		scissorrect.left = 0;                                       // 切り抜き座標左
		scissorrect.right = scissorrect.left + GetwWindow()->width;        // 切り抜き座標右
		scissorrect.top = 0;                                        // 切り抜き座標上
		scissorrect.bottom = scissorrect.top + GetwWindow()->height;       // 切り抜き座標下

		GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

		RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
		break;
	}
	}
}

void GameScene::DrawSprite()
{
	//spr.Draw();

	switch (renderMode)
	{
	case GameScene::RenderingMode::normal:
		break;
	case GameScene::RenderingMode::foveated:
	{
		outer.sprite.Draw();
		middle.sprite.Draw();
		center.sprite.Draw();

		break;
	}
	case GameScene::RenderingMode::vr:
		for (int eye = 0; eye < 2; eye++)
		{
			eyeRes[eye].outer.sprite.Draw();
			eyeRes[eye].middle.sprite.Draw();
			eyeRes[eye].center.sprite.Draw();
		}
		break;
	default:
		break;
	}
	
}