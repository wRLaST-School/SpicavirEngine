#include "stdafx.h"
#include "BTEditorScene.h"
#include <Input.h>
#include <BTEditor.h>

void BTEditorScene::LoadResources()
{
}

void BTEditorScene::Init()
{
	Camera::Set(cam);
	Camera2D::Set(&cam2d);
}

void BTEditorScene::Update()
{
	Camera2D::OnFrameStart();

	if (Input::Mouse::Down(Click::Middle)) {
		Float2 v;
		v = Input::Mouse::GetVel();
		cam2d.x -= (int32_t)v.x;
		cam2d.y -= (int32_t)v.y;
	}
}

void BTEditorScene::DrawBack()
{
}

void BTEditorScene::Draw3D()
{
}

void BTEditorScene::DrawSprite()
{
	BTEditor::Draw();
}
