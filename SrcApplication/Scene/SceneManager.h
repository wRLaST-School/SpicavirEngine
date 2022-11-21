#pragma once
#include "IScene.h"
#include "Essentials.h"
class SceneManager final
{
public:
	static void Init();
	static void Update();
	static void Draw3D();
	static void DrawSprite();
	static void DrawBack();

	//�񓯊��Ŏ��̃V�[���̓ǂݍ��݂��J�n����
	template <class NextScene> static void LoadScene();

	//�ǂݍ��݂��I����Ă�����V�[����؂�ւ��A�I����Ă��Ȃ��Ȃ牽�����Ȃ�
	static void Transition();

	enum class LoadState {
		NotInProgress,
		Loading,
		Loaded
	};
	//���݂̃V�[���ǂݍ��݂̏�Ԃ��擾
	static LoadState GetLoadState();

	static unique_ptr<IScene> currentScene;
	static unique_ptr<IScene> nextScene;

private:
	//���[�h�̏�ԁA���t���[���̍ŏ��ɍX�V�����
	static LoadState loadState;
	//�������̓��A���^�C���X�V�A���t���[���̍ŏ��Ƀ��Z�b�g
	static bool loadFinished;
	static void UpdateLoadState();

//�ȉ���{�g�p�֎~
public:
	//�񓯊��ł̃V�[���ǂݍ��݂��s�킸�ɒ��ڃV�[���؂�ւ�������(�o�O�N�������Ȃ̂Œ���)
	template <class NextScene> static void InstantTransition();

private:
	SceneManager();
	~SceneManager() {};
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager& a) = delete;
};

