#pragma once
#include <Vec3.h>
class Ray
{
public:
	Ray();
	Ray(Vec3 ray, Vec3 origin);

	Vec3 ray;
	Vec3 origin;
};

