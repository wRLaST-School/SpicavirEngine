#include "FrameRate.h"

void FrameRate::FrameStartWithWait()
{
	frameend = std::chrono::system_clock::now();

	double waitTimer = 16.6 - (std::chrono::duration_cast<std::chrono::milliseconds>(frameend - framestart).count());
	if (waitTimer > 0)Sleep((int32_t)waitTimer / 2);
	framestart = std::chrono::system_clock::now();
}

void FrameRate::InitMark()
{
	framestart = std::chrono::system_clock::now();
}

chrono::system_clock::time_point FrameRate::frameend, FrameRate::framestart;