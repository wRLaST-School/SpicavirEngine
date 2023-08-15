#include "FrameRate.h"
#include <thread>

void FrameRate::FrameStartWithWait()
{
	sFrameend = std::chrono::system_clock::now();

	std::chrono::microseconds waitTime = std::chrono::microseconds(16666) - (std::chrono::duration_cast<std::chrono::microseconds>(sFrameend - sFramestart));
	std::this_thread::sleep_for(waitTime);
	sFramestart = std::chrono::system_clock::now();
}

void FrameRate::InitMark()
{
	sFramestart = std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point FrameRate::sFrameend, FrameRate::sFramestart;