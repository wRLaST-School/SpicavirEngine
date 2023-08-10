#pragma once
#include<Object3D.h>
#include <SpEffekseer.h>
class GravSphere
{
public:
	GravSphere(Float3 pos, Vec3 vel, float speed);
	void Update();
	void Draw();

private:
	Float3 pos_;

	float r_ = 2.f;
	float gravR_ = 8.f;

	float maxHomeRad_ = PIf / 180.f;
	Vec3 vel_;
	float speed_;

	Object3D sphere_;

	Effekseer::Handle hnd_;
};

