#pragma once
#include "Object3D.h"

class PlayerBullet
	: public Object3D
{
public:
	PlayerBullet(Float3 position, Vec3 velocity, bool isHoming, bool counter = false);

	void Update();

	Vec3 velocity;

	int timer = 0;

	static const int lifetime = 600;

	bool isHoming = false;

	float r = 10.0f;

	int damage = 100;

	bool del = false;

	bool counter = false;
	float counterSpeed = 20;
};

