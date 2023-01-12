#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <future>
#include <thread>
#include <SoundManager.h>
#include <IPostEffector.h>
#include <Bloom.h>
#include <GameScene.h>
#include <TitleScene.h>

future<void> SceneManager::ftr;

void SceneManager::Init()
{
	BloomP1::Init();
	BloomP2::Init();
	BloomP3::Init();
	BloomFin::Init();
	InstantTransition<TitleScene>();
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
	transitionQueued = true;
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
bool SceneManager::transitionQueued = false;

void SceneManager::TransitionConfirm()
{
	if (loadState == LoadState::Loaded && transitionQueued)
	{
		delete currentScene.release();
		currentScene.swap(nextScene);

		currentScene->Init();

		loadState = LoadState::NotInProgress;

		SpTextureManager::ReleasePerSceneTexture();
		ModelManager::ReleasePerSceneModel();
		SoundManager::ReleasePerSceneSounds();
	}

	transitionQueued = false;
};