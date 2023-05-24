#pragma once
#include <Essentials.h>
#include <Object3D.h>
class LevelManager
{
public:
	static void Init();

	static void Update();

	static void Draw();

	static void Clear();

	static eastl::list<Object3D> objects;
};

