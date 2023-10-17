#pragma once
#include <Essentials.h>
#include <Object3D.h>
class LevelManager : public IComponent
{
public:
	LevelManager();

	void Init();

	void Update();

	void Draw();

	void Clear();
private:
};
