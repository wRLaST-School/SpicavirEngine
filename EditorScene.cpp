#include "stdafx.h"
#include "EditorScene.h"
#include <Toolbar.h>
#include <Input.h>
#include <SpImGui.h>

void EditorScene::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/think.png", "hoge");
}

void EditorScene::Init()
{
	Camera2D::Set(&cam);
}

void EditorScene::Update()
{
	if (Input::Mouse::Down(Click::Middle)) {
		Float2 v;
		v = Input::Mouse::GetVel();
		cam.x -= v.x;
		cam.y -= v.y;
	}
}

void EditorScene::DrawSprite()
{
	cam.DrawRotaGraphCam(0, 0, 1, 1.f, 0.f, "hoge");
}
