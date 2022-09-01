#include "Timer.h"

void Timer::Update()
{
	if (frameTimer >= 60)
	{
		timerSec--;
		frameTimer = 0;
	}
	else
		frameTimer++;
}

void Timer::Draw()
{
	TextDrawer::DrawString(to_string(max(timerSec, 0)), 1180, 100, Align::TopRight, sopt);
}
