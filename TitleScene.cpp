#include "stdafx.h"
#include "TitleScene.h"
#include <Input.h>
#include <Transition.h>
#include <GameScene.h>

void TitleScene::LoadResources()
{
}

void TitleScene::Init()
{
	Camera::Set(cam);
}

void TitleScene::Update()
{
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		Transition::Start<GameScene>();
	}
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawSprite()
{
}
