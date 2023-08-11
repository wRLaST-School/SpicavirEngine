#pragma once
#include<Object3D.h>
#include <SpEffekseer.h>
class GravSphere
{
public:
	GravSphere(Float3 pos, Vec3 vel, float speed,
		float gravR, float gravitySpeed, float maxHomeRad);
	void Update();
	void CheckCollisions();
	void Draw();

private:
	Float3 pos_;

	float r_ = 2.f;
	float gravR_;

	float maxHomeRad_;
	Vec3 vel_;
	float speed_;

	float gravSpeed_;

	Object3D sphere_;

	Effekseer::Handle hnd_;
};

