#include "stdafx.h"
#include "SingleCamTestScene.h"
#include <TextDrawer.h>
#include <LineDrawer.h>
#include <Input.h>

void SingleCamTestScene::LoadResources()
{
	ModelManager::Register("cube", "Cube");
	ModelManager::Register("square", "Pane");
	ModelManager::Register("skydome", "Sky");
}

void SingleCamTestScene::Init()
{
	camera.UseDefaultParams();

	pane.model = ModelManager::GetModel("Pane");
	sky.model = ModelManager::GetModel("Sky");

	e1.position.x = 4;
	e1.position.z = 3;
	e1.radius = {3.f, 3.f, 3.f};

	e2.position.x = -4;
	e2.position.z = 3;
	e2.radius = { 3.f, 3.f, 3.f };
	e2.shape = Emitter<IParticle>::Shape::Sphere;
}

void SingleCamTestScene::Update()
{
	e1.Update();
	e2.Update();

	if (Input::Key::Down(DIK_SPACE) || Input::Pad::Down(Button::R))
	{
		e1.DrawEmitArea();
		e2.DrawEmitArea();
	}

	camera.UpdateMatrix();
	pane.UpdateMatrix();
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
	e1.Draw();
	e2.Draw();

	LineDrawer::DrawAllLines();
}

void SingleCamTestScene::DrawSprite()
{
	//TextDrawer::DrawString("HOGE", 0, 0, Align::TopLeft);
}
