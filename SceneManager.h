#pragma once
#include "IScene.h"
#include "Essentials.h"
class SceneManager
{
public:
	void Init();
	void Update();
	void Draw();
	unique_ptr<IScene> currentScene;
};

