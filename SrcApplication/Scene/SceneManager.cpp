#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <future>
#include <thread>
#include <SoundManager.h>

static future<void> ftr;

void SceneManager::Init()
{
	InstantTransition<TestScene>();
}

void SceneManager::Update()
{
	FrameRate::FrameStartWithWait();
	UpdateLoadState();

	if (Input::Key::Triggered(DIK_T))
	{
		LoadScene<TestScene>();
		Transition();
	}

	if (Input::Key::Triggered(DIK_N))
	{
		LoadScene<SingleCamTestScene>();
		Transition();
	}

	if (Input::Key::Triggered(DIK_R))
	{
		InstantTransition<SingleCamTestScene>();
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

void SceneManager::Transition()
{
	if (loadState == LoadState::Loaded)
	{
		delete currentScene.release();
		currentScene.swap(nextScene);

		currentScene->Init();

		loadState = LoadState::NotInProgress;

		ftr = std::async(std::launch::async, [&] {
				SpTextureManager::ReleasePerSceneTexture();
				ModelManager::ReleasePerSceneModel();
				SoundManager::ReleasePerSceneSounds();
			});
	}
}

SceneManager::LoadState SceneManager::GetLoadState()
{
	return loadState;
}

template <class NextScene> void SceneManager::InstantTransition()
{
	delete currentScene.release();
	Light::Init();
	currentScene = unique_ptr<IScene>(new NextScene());
	currentScene->LoadResources();
	currentScene->Init();
	FrameRate::InitMark();
}

template<class NextScene>
void SceneManager::LoadScene()
{
	if(loadState != LoadState::NotInProgress)
	{
		return;
	}

	nextScene = unique_ptr<IScene>(new NextScene());

	ftr = std::async(std::launch::async, [&] {
			SpTextureManager::PreLoadNewScene();
			ModelManager::PreLoadNewScene();
			SoundManager::PreLoadNewScene();
			nextScene->LoadResources();
			loadFinished = true;
		});
	loadState = LoadState::Loading;
}

void SceneManager::UpdateLoadState()
{
	if (loadState == LoadState::Loading && loadFinished)
	{
		loadState = LoadState::Loaded;
		loadFinished = false;
	}
}

unique_ptr<IScene> SceneManager::currentScene = nullptr;
unique_ptr<IScene> SceneManager::nextScene = nullptr;
SceneManager::LoadState SceneManager::loadState = SceneManager::LoadState::NotInProgress;
bool SceneManager::loadFinished = false;