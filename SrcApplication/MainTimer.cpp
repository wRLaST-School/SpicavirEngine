#include "stdafx.h"
#include "MainTimer.h"
#include <SpDS.h>
#include <Util.h>

void MainTimer::Load()
{
	SpTextureManager::LoadDiv("Resources/Numbers.png", 115, 93, 10, 1, sNumbers);
}

void MainTimer::Init()
{
	timerSec = 120;
	sFrameTimer = 60;
}

void MainTimer::Update()
{
	if (sDamageTimer > 0)
	{
		sDamageTimer--;
	}

	if (sFrameTimer >= 60)
	{
		timerSec--;
		sFrameTimer = 0;
	}
	else
		sFrameTimer++;
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
			sNumbers.at(digis[i]), Anchor::Center,
			(sDamageTimer % 20 >= 10) ? Color::Red : Color::White);
	}
}

void MainTimer::Damage()
{
	sDamageTimer = 60;
}

int32_t MainTimer::timerSec = 120;

int32_t MainTimer::sFrameTimer = 60;

int32_t MainTimer::sDamageTimer = 0;

std::vector<TextureKey> MainTimer::sNumbers ={
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