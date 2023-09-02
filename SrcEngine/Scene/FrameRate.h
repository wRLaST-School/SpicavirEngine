#pragma once
#include "Essentials.h"
#include <chrono>
#include <Util.h>
class FrameRate
{
public:
	static void FrameStartWithWait();
	static void InitMark();

private:
	static std::chrono::system_clock::time_point  sFrameend, sFramestart;

	static class WaitTimeData {
	public:
		int32_t waitTimeBase_ = 16666;

		template <class Archive>
		void serialize(Archive& ar) {
			ar(waitTimeBase_);
		}

	} sWaitTimeData;
};

