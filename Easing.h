#pragma once
class Easing
{
	//tは0-1、expは指数
	Vec2 Out(Vec2 start, Vec2 end, float t, float exp = 3);
	//tは0-1、expは指数
	Vec2 In(Vec2 start, Vec2 end, float t, float exp = 3);
	//tは0-1、expは指数
	Vec2 InOut(Vec2 start, Vec2 end, float t);

	//tは0-1、expは指数
	float Out(float start, float end, float t, float exp = 3);
	//tは0-1、expは指数
	float In(float start, float end, float t, float exp = 3);
	//tは0-1、expは指数
	float InOut(float start, float end, float t);

	//tは0-1、expは指数、戻り値は0-1
	float Out(float t, float exp = 3);
	//tは0-1、expは指数、戻り値は0-1
	float In(float t, float exp = 3);
	//tは0-1、戻り値は0-1
	float InOut(float t);
};

