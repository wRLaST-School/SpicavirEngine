#include "SceneManager.h"
#include "GameScene.h"

void SceneManager::Init()
{
	currentScene = unique_ptr<GameScene>(new GameScene());
	currentScene->Init();
}

void SceneManager::Update()
{
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
