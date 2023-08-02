#include "FrameRate.h"

void FrameRate::FrameStartWithWait()
{
	sFrameend = std::chrono::system_clock::now();

	double waitTimer = 16.6 - (std::chrono::duration_cast<std::chrono::milliseconds>(sFrameend - sFramestart).count());
	if (waitTimer > 0)Sleep((int32_t)waitTimer);
	sFramestart = std::chrono::system_clock::now();
}

void FrameRate::InitMark()
{
	sFramestart = std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point FrameRate::sFrameend, FrameRate::sFramestart;