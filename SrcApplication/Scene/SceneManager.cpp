#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <IPostEffector.h>
#include <Bloom.h>

future<void> SceneManager::ftr;
bool SceneManager::transitionQueued = false;

void SceneManager::Init()
{
	InstantTransition<SingleCamTestScene>();
	//InstantTransition<GameScene>();
}

void SceneManager::Update()
{
	ConfirmTransition();
	FrameRate::FrameStartWithWait();
	UpdateLoadState();

	Transition();

	if (Input::Key::Triggered(DIK_T) && Input::Key::Down(DIK_LSHIFT))
	{
		LoadScene<TestScene>();
		Transition();
	}

	if (Input::Key::Triggered(DIK_N) && Input::Key::Down(DIK_LSHIFT))
	{
		LoadScene<SingleCamTestScene>();
		Transition();
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
		transitionQueued = true;
	}
}

void SceneManager::ConfirmTransition()
{
	if (transitionQueued)
	{
		delete currentScene.release();
		currentScene.swap(nextScene);

		currentScene->Init();

		loadState = LoadState::NotInProgress;

		SpTextureManager::ReleasePerSceneTexture();
		ModelManager::ReleasePerSceneModel();
		SoundManager::ReleasePerSceneSounds();

		transitionQueued = false;
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