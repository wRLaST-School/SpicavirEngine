#pragma once
#include "Essentials.h"
#include <chrono>
class FrameRate
{
public:
	static void FrameStartWithWait();
	static void InitMark();

private:
	static std::chrono::system_clock::time_point  sFrameend, sFramestart;
};

