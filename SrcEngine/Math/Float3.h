#pragma once
#include "Float4.h"
#include <Vec3.h>

struct Float3
{
	float x;
	float y;
	float z;

	void operator = (const Float3& f3)
	{
		x = f3.x; y = f3.y; z = f3.z;
	};

	Float3& operator +=(const Float3& other) 
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

		return *this;
	};

	operator Vec3() const { return Vec3(x, y, z); }


	operator Float4()
	{
		return { x, y, z, 0 };
	}
};