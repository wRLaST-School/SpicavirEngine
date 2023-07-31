#pragma once
#include <SpTextureManager.h>
class MainTimer
{
public:
	static void Load();
	static void Init();

	static void Update();
	static void Draw();

	static void Damage();

	static int32_t timerSec;

private:
	static int32_t frameTimer;
	static const int32_t frameTime = 60;
	static std::vector<TextureKey> numbers;
	static int32_t damageTimer;
};