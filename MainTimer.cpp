#include "stdafx.h"
#include "MainTimer.h"
#include <SpDS.h>
#include <Util.h>

void MainTimer::Load()
{
	SpTextureManager::LoadDiv("Resources/Numbers.png", 115, 93, 10, 1, numbers);
}

void MainTimer::Init()
{
	timerSec = 120;
	frameTimer = 60;
}

void MainTimer::Update()
{
	if (frameTimer >= 60)
	{
		timerSec--;
		frameTimer = 0;
	}
	else
		frameTimer++;
}

void MainTimer::Draw()
{
	static Float2 drawPosTR = { 100.f, 100.f };

	static float numWidth = 115;

	timerSec = Util::ClampMin(timerSec, 0);

	int32_t digis[3]{ timerSec % 10,

	timerSec / 10 % 10,

	timerSec / 100 };

	for (int32_t i = 0; i < 3; i++)
	{
		SpDS::DrawRotaGraph(
			(int32_t)((GetSpWindow()->width - drawPosTR.x) - (float)numWidth * ((float)i + 0.5f)),
			(int32_t)drawPosTR.y + 48,
			1.f, 1.f, 0.f,
			numbers.at(digis[i]));
	}
}

int32_t MainTimer::timerSec = 120;

int32_t MainTimer::frameTimer = 60;

std::vector<TextureKey> MainTimer::numbers ={
		"num_0",
		"num_1",
		"num_2",
		"num_3",
		"num_4",
		"num_5",
		"num_6",
		"num_7",
		"num_8",
		"num_9"
};