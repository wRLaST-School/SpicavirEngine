#pragma once
#include "Object3D.h"

class PlayerBullet
	: public Object3D
{
public:
	PlayerBullet(Float3 position, Vec3 velocity):velocity(velocity) {
		this->scale = { 10, 10, 10 };
		this->position = position;
	};

	void Update();

	Vec3 velocity;

	int timer = 0;

	static const int lifetime = 600;
};

