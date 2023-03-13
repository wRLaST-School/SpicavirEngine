#include "stdafx.h"
#include "EditorScene.h"
#include <Toolbar.h>
#include <Input.h>
#include <SpImGui.h>
#include <ToolManager.h>
#include <MapChip.h>
#include <Grid.h>
#include <SnakeBody.h>
#include <SnakeHead.h>

void EditorScene::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/think.png", "hoge");
	SpTextureManager::LoadTexture("Resources/white.png", "white");
	MapChip::LoadResource();
}

void EditorScene::Init()
{
	Camera2D::Set(&cam);
	MapChip::Init(32, 18);
}

void EditorScene::Update()
{
	if (Input::Mouse::Down(Click::Middle)) {
		Float2 v;
		v = Input::Mouse::GetVel();
		cam.x -= v.x;
		cam.y -= v.y;
	}

	ToolManager::Update();
}

void EditorScene::DrawSprite()
{
	Grid::Draw();

	//cam.DrawRotaGraphCam(0, 0, 1, 1.f, 0.f, "hoge");
	MapChip::Draw();

	SnakeHead::DrawAll();
	SnakeBody::DrawAll();

	Grid::DrawOver();

	ToolManager::Draw();
}
