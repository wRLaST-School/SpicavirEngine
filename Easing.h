#pragma once
class Easing
{
	//t��0-1�Aexp�͎w��
	Vec2 Out(Vec2 start, Vec2 end, float t, float exp = 3);
	//t��0-1�Aexp�͎w��
	Vec2 In(Vec2 start, Vec2 end, float t, float exp = 3);
	//t��0-1�Aexp�͎w��
	Vec2 InOut(Vec2 start, Vec2 end, float t);

	//t��0-1�Aexp�͎w��
	float Out(float start, float end, float t, float exp = 3);
	//t��0-1�Aexp�͎w��
	float In(float start, float end, float t, float exp = 3);
	//t��0-1�Aexp�͎w��
	float InOut(float start, float end, float t);

	//t��0-1�Aexp�͎w���A�߂�l��0-1
	float Out(float t, float exp = 3);
	//t��0-1�Aexp�͎w���A�߂�l��0-1
	float In(float t, float exp = 3);
	//t��0-1�A�߂�l��0-1
	float InOut(float t);
};

