#pragma once
#include "Vec2.h"
struct DLLExport Float2
{
	float x;
	float y;

	[[maybe_unused]]
	void operator = (const Float2& f2)
	{
		x = f2.x; y = f2.y;
	};

	[[maybe_unused]]
	operator Vec2() const { return Vec2(x, y); };
};