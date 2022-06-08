#pragma once
#include "IScene.h"
#include "Essentials.h"
class SceneManager
{
public:
	void Init();
	void Update();
	void Draw3D();
	void DrawSprite();
	void DrawBack();
	unique_ptr<IScene> currentScene;
};

