#include "stdafx.h"
#include "SceneFromFile.h"
#include <SceneRW.h>

SceneFromFile::SceneFromFile(std::string filePath)
{
	filePath_ = filePath;
}

void SceneFromFile::LoadResources()
{
	SceneRW::LoadScene(this, filePath_);
}

void SceneFromFile::Init()
{
	for (auto& c : components_)
	{
		InitAllChildComponents(c.second.get());
	}
}

void SceneFromFile::Update()
{
}

void SceneFromFile::DrawBack()
{
}

void SceneFromFile::Draw3D()
{
}

void SceneFromFile::DrawSprite()
{
}
