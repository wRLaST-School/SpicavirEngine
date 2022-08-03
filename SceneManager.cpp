#include "SceneManager.h"
#include "GameScene.h"
#include "TestScene.h"
#include "Input.h"

void SceneManager::Init()
{
	//currentScene = unique_ptr<GameScene>(new GameScene());

	Transition<TestScene>();
}

void SceneManager::Update()
{
	currentScene->Update();

	if (Input::Key::Triggered(DIK_R))
	{
		Transition<GameScene>();
	}
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

template <class NextScene> void SceneManager::Transition()
{
	currentScene.release();
	Light::Init();
	currentScene = unique_ptr<NextScene>(new NextScene());
	currentScene->Init();
}
