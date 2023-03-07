#include "stdafx.h"
#include "TimeManager.h"

float DeltaTime()
{
    return TimeManager::GetDelta();
}

float TimeManager::GetDelta()
{
    return delta;
}

void TimeManager::FrameStart()
{
    if (started)
    {
        auto start = chrono::system_clock::now();
        auto t = start - lastframe;
        delta = (float)chrono::duration_cast<chrono::milliseconds>(t).count() / 1000.f;
        delta *= gameSpeed;
        lastframe = start;
        return;
    }

    delta = 0.001f;
    lastframe = chrono::system_clock::now();
    started = true;
}

void TimeManager::Init()
{
    started = false;
}

void TimeManager::SetGameSpeed(float spd)
{
}

chrono::system_clock::time_point TimeManager::lastframe;
float TimeManager::delta;
float TimeManager::gameSpeed;

bool TimeManager::started;
