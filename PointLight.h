#pragma once
#include "wMath.h"
class PointLight
{
public:
	PointLight(Float3 pos, Float3 color, Float3 att) :pos(pos), color(color), att(att){};
	Float3 pos;
	Float3 color;
	Float3 att;

	bool isActive;
};
typedef unsigned int PointLightHandle;

