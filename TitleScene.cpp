#include "stdafx.h"
#include "TitleScene.h"
#include "SoundManager.h"
#include "Input.h"
#include <TextDrawer.h>
#include <SceneManager.h>
#include <GameScene.h>

void TitleScene::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/title.png", "Title");
}

void TitleScene::Init()
{
	title = Sprite("Title");
	title.position = { 960, 540 };
}

void TitleScene::Update()
{
	timer++;
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		SceneManager::LoadScene<GameScene>();
	}

	SceneManager::Transition();

	title.position.y = 540 + sin((float)timer / 50 ) * 10;

	title.UpdateMatrix();
}

void TitleScene::DrawBack()
{
	
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawSprite()
{
	
	title.Draw();

	if (SceneManager::GetLoadState() == SceneManager::LoadState::Loading)
	{
		
	}
}