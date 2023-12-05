#include "stdafx.h"
#include "MainTimer.h"
#include <SpDS.h>
#include <Util.h>
#include <GameManager.h>

void MainTimer::Load()
{
	SpTextureManager::LoadDiv("Assets/Images/Numbers.png", 115, 93, 10, 1, numbers_);
}

void MainTimer::Init()
{
	timerSec = 120;
	frameTimer_ = 60;
}

void MainTimer::Update()
{
	if (!GameManager::sDebugImmunity)
	{
		if (damageTimer_ > 0)
		{
			damageTimer_--;
		}

		if (frameTimer_ >= 60)
		{
			timerSec--;
			frameTimer_ = 0;
		}
		else
			frameTimer_++;
	}
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
			numbers_.at(digis[i]), Anchor::Center,
			(damageTimer_ % 20 >= 10) ? Color::Red : Color::White);
	}
}

void MainTimer::Damage()
{
	damageTimer_ = 60;
}