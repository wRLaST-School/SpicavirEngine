#include "stdafx.h"
#include "LevelManager.h"
#include <LevelLoader.h>

LevelManager::LevelManager()
{
	Init();
}

void LevelManager::Init()
{
	Clear();
	LevelLoader::Load("Levels/scene.json", this);
}

void LevelManager::Update()
{
	eastl::list<Object3D*> objects = GetComponents<Object3D>("Object3D");

	for (auto& o : objects)
	{
		o->UpdateMatrix();
	}
}

void LevelManager::Draw()
{
	eastl::list<Object3D*> objects = GetComponents<Object3D>("Object3D");

	for (auto& o : objects)
	{
		o->Draw();
	}
}

void LevelManager::Clear()
{
	ClearComponentWithKey("Object3D");
}