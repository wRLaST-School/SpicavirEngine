#pragma once
#include <Essentials.h>
#include <LevelManager.h>

class LevelLoader
{
public:
	static void Load(std::string path, LevelManager* lm);
};

