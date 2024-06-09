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
#include <SceneRW.h>
#include <SceneFromFile.h>

std::future<void> SceneManager::ftr;
bool SceneManager::transitionQueued = false;

void SceneManager::Init()
{
	Transition::Load();
	InstantTransition<TestScene>();
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
		/*if (Input::Key::Triggered(DIK_G))
		{
			LoadScene<GameScene>();
		}

		if (Input::Key::Triggered(DIK_T))
		{
			LoadScene<TitleScene>();
		}*/

		if (Input::Key::Triggered(DIK_D))
		{
			GameManager::sShowDebug = !GameManager::sShowDebug;
		}

		//if (Input::Key::Triggered(DIK_E))
		//{
		//	LoadScene<BTEditorScene>();
		//}

		//Transition();
	}

	if (!GameManager::sDebugTimeStop)
	{
		IComponent::UpdateAllChildComponents(currentScene.get());
	}
}

void SceneManager::Draw3D()
{
	currentScene->Draw3D();
	IComponent::DrawAllChildComponents(currentScene.get());
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

void SceneManager::WaitForLoadAndTransition()
{
	while (loadState == LoadState::Loading)
	{
		UpdateLoadState();
		Sleep(16);
	}

	transitionQueued = true;
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

		Light::ClearAllPointLights();

		transitionQueued = false;
	}
}

void SceneManager::ReleaseScene()
{
	currentScene.reset();
	nextScene.reset();
}

SceneManager::LoadState SceneManager::GetLoadState()
{
	return loadState;
}

IScene* SceneManager::GetScene()
{
	return currentScene.get();
}

template <class NextScene,  class... Args> void SceneManager::InstantTransition(Args... args)
{
	currentScene.release();
	currentScene = nullptr;
	Light::Init();
	currentScene = std::make_unique<NextScene>(args...);
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