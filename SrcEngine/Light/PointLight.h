#pragma once
#include "SpMath.h"
#include "Essentials.h"
class PointLight
{
public:
	PointLight() :pos({ 0,0,0 }), color({ 0,0,0 }), att({ 0,0,0 }) { isActive = false; }
	PointLight(Float3 pos, Float3 color, Float3 att) :pos(pos), color(color), att(att), isActive(true) {}
	Float3 pos;
	Float3 color;
	Float3 att;

	void DrawFrame();
	void DrawLightEditor();

	bool isActive = false;

public:
	struct CBData {
		Float3 pos;
		float padding1;
		Float3 color;
		float padding;
		Float3 att;
		uint32_t isActive = false;
	};

	//change MAX_PLIGHTS in hlsli aswell
	static const uint32_t MAX_LIGHTS = 16;

	//点光源数が上限を超えた場合のエラー
	class QuantityOverflow {
	public:
		QuantityOverflow(int32_t limit, int32_t actual):limit(limit),actual(actual) {};
		int32_t limit;
		int32_t actual;
	};

	string name = "unnamed";

private:
	bool showFrame = false;
};

typedef string PointLightKey;

