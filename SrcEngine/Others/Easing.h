
#pragma once

class Easing {
public:
	static float In(float t, float exp = 3);
	static float Out(float t, float exp = 3);

	static Vec3 In(Vec3 zero, Vec3 one, float t, float exp = 3);
	static Vec3 Out(Vec3 zero, Vec3 one, float t, float exp = 3);
};

