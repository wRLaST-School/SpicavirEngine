#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>

void SingleCamTestScene::LoadResources()
{
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("skydome", "Sky");
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();
	//camera.useWindowSize = false;
	camera.renderWidth = 300;
	camera.renderHeight = 600;

	obj.model = ModelManager::GetModel("Cube");
	sky.model = ModelManager::GetModel("Sky");
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

	//obj.Draw();
	//sky.Draw();

	LineDrawer::DrawCube({0, 0, 5}, {2,2,2}, {1,1,1,1});
	LineDrawer::DrawAllLines();
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
