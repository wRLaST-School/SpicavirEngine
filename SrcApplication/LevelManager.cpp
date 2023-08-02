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
	for (auto& o : objects_)
	{
		o.UpdateMatrix();
	}
}

void LevelManager::Draw()
{
	for (auto& o : objects_)
	{
		o.Draw("white");
	}
}

void LevelManager::Clear()
{
	objects_.clear();
}

eastl::list<Object3D> LevelManager::objects_;