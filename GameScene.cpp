#include "stdafx.h"
#include "GameScene.h"
#include <TextDrawer.h>

void GameScene::Init()
{
	SpTextureManager::Init();
	camera.UseDefaultParams();
	camera.renderWidth = 300;
	camera.renderHeight = 600;

	mCube = Model("cube");
	mSky = Model("skydome");

	obj.model = &mCube;
	sky.model = &mSky;
}

void GameScene::Update()
{
	camera.UpdateMatrix();
	obj.UpdateMatrix();
	sky.UpdateMatrix();
}

void GameScene::DrawBack()
{
}

void GameScene::Draw3D()
{
	Camera::Set(camera);

	obj.Draw();
	sky.Draw();
}

void GameScene::DrawSprite()
{
	TextDrawer::DrawString("hogeÇŸÇ∞Å[Å[", 100, 100, Align::TopLeft);
}
