#include "SceneManager.h"
#include "GameScene.h"
#include "Input.h"

void SceneManager::Init()
{
	currentScene = unique_ptr<GameScene>(new GameScene());
	currentScene->Init();
}

void SceneManager::Update()
{
	if (Input::Key::Triggered(DIK_R))
	{
		currentScene.release();
		currentScene = unique_ptr<GameScene>(new GameScene());
		currentScene->Init();
	}
	currentScene->Update();
}

void SceneManager::Draw3D()
{
	currentScene->Draw3D();
}

void SceneManager::DrawSprite()
{
	currentScene->DrawSprite();
}

void SceneManager::DrawBack()
{
	currentScene->DrawBack();
}
