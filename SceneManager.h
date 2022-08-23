#pragma once
#include "IScene.h"
#include "Essentials.h"
#include <chrono>
class SceneManager
{
public:
	void Init();
	void Update();
	void Draw3D();
	void DrawSprite();
	void DrawBack();

	template <class NextScene> void Transition();

	unique_ptr<IScene> currentScene = nullptr;

private:
	chrono::system_clock::time_point  frameend, framestart;
};

