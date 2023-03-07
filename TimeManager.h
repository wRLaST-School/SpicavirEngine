#pragma once
#include <chrono>
#include <Essentials.h>

class TimeManager
{
public:
	static float GetDelta();
	static void FrameStart();
	static void Init();

	static void SetGameSpeed(float spd = 1.0f);

private:
	static chrono::system_clock::time_point lastframe;
	static float delta;
	static float gameSpeed;

	static bool started;
};

float DeltaTime();
