#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();
	//camera.useWindowSize = false;
	camera.renderWidth = 300;
	camera.renderHeight = 600;

	mCube = Model("cube");
	mSky = Model("skydome");

	obj.model = &mCube;
	sky.model = &mSky;
}

void SingleCamTestScene::Update()
{
	camera.UpdateMatrix();
	obj.UpdateMatrix();
	sky.UpdateMatrix();
}

void SingleCamTestScene::DrawBack()
{

}

void SingleCamTestScene::Draw3D()
{
	Camera::Set(camera);

	obj.Draw();
	sky.Draw();
}

void SingleCamTestScene::DrawSprite()
{
	TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
