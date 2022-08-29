#include "SceneManager.h"
#include "GameScene.h"
#include "TestScene.h"
#include "Input.h"
#include "MonkeyScene.h"
#include "ResultScene.h"

void SceneManager::Init()
{
	Transition<GameScene>();
	framestart = std::chrono::system_clock::now();
}

void SceneManager::Update()
{
	frameend = std::chrono::system_clock::now();

	double waitTimer = 16.6 - (std::chrono::duration_cast<std::chrono::milliseconds>(frameend - framestart).count());
	if(waitTimer > 0)Sleep((int)waitTimer / 2);
	//OutputDebugString((to_wstring(waitTimer) + L"\nat:" + to_wstring(std::chrono::duration_cast<std::chrono::milliseconds>(frameend - framestart).count()) + L"\n").c_str());
	framestart = std::chrono::system_clock::now();

	currentScene->Update();

	if (Input::Key::Triggered(DIK_R))
	{
		Transition<MonkeyScene>();
	}
	else if (Input::Key::Triggered(DIK_T))
	{
		Transition<TestScene>();
	}

	if (currentScene->sceneId == "Game")
	{
		if (currentScene->endScene)
		{
			GameScene& scene = dynamic_cast<GameScene&>(*currentScene);
			GameSceneOutputs gso = scene.GetTransitionData();
			Transition<ResultScene>();
			ResultScene& scene2 = dynamic_cast<ResultScene&>(*currentScene);
			scene2.GiveTransitionData(gso);
		}
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
}
