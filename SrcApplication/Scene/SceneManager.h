#pragma once
#include "IScene.h"
#include "Essentials.h"
#include <future>
#include <thread>
#include <SoundManager.h>
#include <SpEffekseer.h>
class SceneManager final
{
public:
	static void Init();
	static void Update();
	static void Draw3D();
	static void DrawSprite();
	static void DrawBack();

	//非同期で次のシーンの読み込みを開始する
	template <class NextScene> static void LoadScene()
	{
		if (loadState != LoadState::NotInProgress)
		{
			return;
		}

		nextScene = std::make_unique<NextScene>();

		ftr = std::async(std::launch::async, [&] {
			SpTextureManager::PreLoadNewScene();
			ModelManager::PreLoadNewScene();
			SoundManager::PreLoadNewScene();
			SpEffekseer::PreLoadNewScene();
			nextScene->LoadResources();
			loadFinished = true;
			});
		loadState = LoadState::Loading;
	};

	//読み込みが終わっていたらシーンを切り替え、終わっていないなら何もしない
	static void Transition();

	static void ConfirmTransition();

	enum class LoadState {
		NotInProgress,
		Loading,
		Loaded
	};
	//現在のシーン読み込みの状態を取得
	static LoadState GetLoadState();

	static std::unique_ptr<IScene> currentScene;
	static std::unique_ptr<IScene> nextScene;

private:
	//ロードの状態、毎フレームの最初に更新される
	static LoadState loadState;
	//こっちはリアルタイム更新、次フレームの最初にリセット
	static bool loadFinished;
	static void UpdateLoadState();
	static bool transitionQueued;

	//以下基本使用禁止
public:
	//非同期でのシーン読み込みを行わずに直接シーン切り替えをする(バグ起きがちなので注意)
	template <class NextScene> static void InstantTransition();

private:
	SceneManager();
	~SceneManager() {};
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager& a) = delete;
	static std::future<void> ftr;
};