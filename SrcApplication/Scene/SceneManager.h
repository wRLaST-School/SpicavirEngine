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

	//�񓯊��Ŏ��̃V�[���̓ǂݍ��݂��J�n����
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

	//�ǂݍ��݂��I����Ă�����V�[����؂�ւ��A�I����Ă��Ȃ��Ȃ牽�����Ȃ�
	static void Transition();

	static void ConfirmTransition();

	enum class LoadState {
		NotInProgress,
		Loading,
		Loaded
	};
	//���݂̃V�[���ǂݍ��݂̏�Ԃ��擾
	static LoadState GetLoadState();

	static std::unique_ptr<IScene> currentScene;
	static std::unique_ptr<IScene> nextScene;

private:
	//���[�h�̏�ԁA���t���[���̍ŏ��ɍX�V�����
	static LoadState loadState;
	//�������̓��A���^�C���X�V�A���t���[���̍ŏ��Ƀ��Z�b�g
	static bool loadFinished;
	static void UpdateLoadState();
	static bool transitionQueued;

	//�ȉ���{�g�p�֎~
public:
	//�񓯊��ł̃V�[���ǂݍ��݂��s�킸�ɒ��ڃV�[���؂�ւ�������(�o�O�N�������Ȃ̂Œ���)
	template <class NextScene> static void InstantTransition();

private:
	SceneManager();
	~SceneManager() {};
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager& a) = delete;
	static std::future<void> ftr;
};