#pragma once
#include "Essentials.h"
#include <chrono>
class FrameRate
{
public:
	static void FrameStartWithWait();
	static void InitMark();

private:
	static chrono::system_clock::time_point  frameend, framestart;
};
