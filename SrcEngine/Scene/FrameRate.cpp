#include "FrameRate.h"
#include <thread>
#include <SpImGui.h>
#include <Util.h>
#include <GameManager.h>
#include <time.h>
#include <mmsystem.h>

void FrameRate::FrameStartWithWait()
{
	if (GameManager::sShowDebug)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Frame Rate"))
			{
				ImGui::InputInt("Micro Seconds per Frame", &sWaitTimeData.waitTimeBase_);

				if (ImGui::Button("Save"))
				{
					Util::SerializeData("Resources/Data/frameRate.bin", sWaitTimeData);
				}
			}
			ImGui::End();
			});
	}
	timeBeginPeriod(1);

	sFrameend = std::chrono::system_clock::now();

	std::chrono::microseconds waitTime = std::chrono::microseconds(sWaitTimeData.waitTimeBase_) - (std::chrono::duration_cast<std::chrono::microseconds>(sFrameend - sFramestart));
	std::this_thread::sleep_for(waitTime);
	sFramestart = std::chrono::system_clock::now();
	timeEndPeriod(1);
}

void FrameRate::InitMark()
{
	sFramestart = std::chrono::system_clock::now();

	Util::DeserializeData("Resources/Data/frameRate.bin", sWaitTimeData);
}

std::chrono::system_clock::time_point FrameRate::sFrameend, FrameRate::sFramestart;
FrameRate::WaitTimeData FrameRate::sWaitTimeData;