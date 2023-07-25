#include "stdafx.h"
#include "Easing.h"

float Easing::Out(float t, float exp)
{
	return 1 - powf((1 - t), exp);
}

Vec3 Easing::Out(Vec3 zero, Vec3 one, float t, float exp)
{
	return Vec3::Lerp(zero, one, Out(t,exp));
}
