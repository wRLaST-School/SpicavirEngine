#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <GameScene.h>

void SceneManager::Init()
{
	Transition<GameScene>();
}

void SceneManager::Update()
{
	FrameRate::FrameStartWithWait();
	currentScene->Update();

	if (Input::Key::Triggered(DIK_T))
	{
		Transition<TestScene>();
	}

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
	delete currentScene.release();
	Light::Init();
	currentScene = unique_ptr<NextScene>(new NextScene());
	currentScene->Init();
	FrameRate::InitMark();
}
