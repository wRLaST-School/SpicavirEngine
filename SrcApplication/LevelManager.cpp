#include "stdafx.h"
#include "LevelManager.h"
#include <LevelLoader.h>

void LevelManager::Init()
{
	Clear();
	LevelLoader::Load("Levels/scene.json");
}

void LevelManager::Update()
{
	for (auto& o : objects)
	{
		o.UpdateMatrix();
	}
}

void LevelManager::Draw()
{
	for (auto& o : objects)
	{
		o.Draw();
	}
}

void LevelManager::Clear()
{
	objects.clear();
}

eastl::list<Object3D> LevelManager::objects;