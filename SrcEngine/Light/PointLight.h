#pragma once
#include "SpMath.h"
#include "Essentials.h"
class PointLight
{
public:
	PointLight() { isActive = false; }
	PointLight(Float3 pos, Float3 color, Float3 att) :pos(pos), color(color), att(att), isActive(true) {}
	Float3 pos;
	Float3 color;
	Float3 att;

	bool isActive = false;

public:
	struct CBData {
		Float3 pos;
		float padding1;
		Float3 color;
		float padding;
		Float3 att;
		unsigned int isActive = false;
	};

	//change MAX_PLIGHTS in hlsli aswell
	static const unsigned int MAX_LIGHTS = 16;

	//“_ŒõŒ¹”‚ªãŒÀ‚ğ’´‚¦‚½ê‡‚ÌƒGƒ‰[
	class QuantityOverflow {
	public:
		QuantityOverflow(int limit, int actual):limit(limit),actual(actual) {};
		int limit;
		int actual;
	};
};

typedef string PointLightKey;

