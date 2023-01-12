#include "stdafx.h"
#include "ResultScene.h"
#include "SoundManager.h"
#include "Input.h"
#include <TextDrawer.h>
#include <SceneManager.h>
#include <GameScene.h>
#include <TitleScene.h>

void ResultScene::LoadResources()
{
		SpTextureManager::LoadTexture("Resources/success.png", "ResultS");
		SpTextureManager::LoadTexture("Resources/failed.png", "ResultF");
}

void ResultScene::Init()
{
	if (HP::hp > 0)
	{
		gameo = Sprite("ResultS");
	}
	else
	{
		gameo = Sprite("ResultF");
	}
	gameo.position = { 960, 540 };
}

void ResultScene::Update()
{
	timer++;
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		SceneManager::LoadScene<TitleScene>();
	}

	SceneManager::Transition();

	gameo.position.y = 540 + sin((float)timer / 50) * 10;

	gameo.UpdateMatrix();
}

void ResultScene::DrawBack()
{
}

void ResultScene::Draw3D()
{
}

void ResultScene::DrawSprite()
{
	gameo.Draw();
}
