#pragma once
#include "TextDrawer.h"
class Timer
{
public:
	void Update();
	void Draw ();

	int timerSec = 90;
private:
	int frameTimer = 0;
	int frameTime = 60;

	StringOptions sopt =
	{
		{
			64,
			1000,
			128,
			"ÇlÇr ÉSÉVÉbÉN",
			4
		},
		64,
		2,2
	};
};

