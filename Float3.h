#pragma once
#include "Vec3.h"

struct Float3
{
	float x;
	float y;
	float z;

	void operator = (const Float3& f3)
	{
		x = f3.x; y = f3.y; z = f3.z;
	};

	operator Vec3() const { return Vec3(x, y, z); }
};