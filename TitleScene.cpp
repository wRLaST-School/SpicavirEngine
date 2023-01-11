#include "stdafx.h"
#include "TitleScene.h"
#include "SoundManager.h"
#include "Input.h"
#include <TextDrawer.h>
#include <SceneManager.h>
#include <GameScene.h>

void TitleScene::LoadResources()
{
}

void TitleScene::Init()
{

}

void TitleScene::Update()
{
	timer++;
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		SceneManager::LoadScene<GameScene>();
	}

	SceneManager::Transition();
}

void TitleScene::DrawBack()
{
	
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawSprite()
{
	

	if (SceneManager::GetLoadState() == SceneManager::LoadState::Loading)
	{
		
	}
}