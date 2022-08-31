#pragma once
#include "Object3D.h"
class BossBullet :
    public Object3D
{
public:
	BossBullet(Float3 position, Vec3 velocity, Model* model);

	void Update();

	Vec3 velocity;

	int timer = 0;

	static const int lifetime = 120;

	float r = 10.0f;

	bool del = false;
};
