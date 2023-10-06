#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <IPostEffector.h>
#include <Bloom.h>
#include <GameScene.h>
#include <GlobalTimer.h>
#include <TitleScene.h>
#include <Transition.h>
#include <BTEditorScene.h>
#include <GameManager.h>

std::future<void> SceneManager::ftr;
bool SceneManager::transitionQueued = false;

void SceneManager::Init()
{
	Transition::Load();
	InstantTransition<SingleCamTestScene>();
	//InstantTransition<GameScene>();
}

void SceneManager::Update()
{
	GlobalTimer::Update();
	ConfirmTransition();
	FrameRate::FrameStartWithWait();
	UpdateLoadState();
	Transition::Update();

	//デバッグ用シーン変更
	if (Input::Key::Down(DIK_LSHIFT) || Input::Key::Down(DIK_RSHIFT))
	{
		if (Input::Key::Triggered(DIK_G))
		{
			LoadScene<GameScene>();
		}

		if (Input::Key::Triggered(DIK_T))
		{
			LoadScene<TitleScene>();
		}

		if (Input::Key::Triggered(DIK_D))
		{
			GameManager::sShowDebug = !GameManager::sShowDebug;
		}

		if (Input::Key::Triggered(DIK_E))
		{
			LoadScene<BTEditorScene>();
		}

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
	Transition::Draw();
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
		currentScene.release();
		currentScene = nullptr;
		currentScene.swap(nextScene);

		currentScene->Init();

		loadState = LoadState::NotInProgress;

		SpTextureManager::ReleasePerSceneTexture();
		ModelManager::ReleasePerSceneModel();
		SoundManager::ReleasePerSceneSounds();
		SpEffekseer::ReleasePerSceneEffects();

		transitionQueued = false;
	}
}

SceneManager::LoadState SceneManager::GetLoadState()
{
	return loadState;
}

template <class NextScene> void SceneManager::InstantTransition()
{
	currentScene.release();
	currentScene = nullptr;
	Light::Init();
	currentScene = std::make_unique<NextScene>();
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

std::unique_ptr<IScene> SceneManager::currentScene = nullptr;
std::unique_ptr<IScene> SceneManager::nextScene = nullptr;
SceneManager::LoadState SceneManager::loadState = SceneManager::LoadState::NotInProgress;
bool SceneManager::loadFinished = false;