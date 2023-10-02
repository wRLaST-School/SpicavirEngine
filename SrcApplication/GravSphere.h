#pragma once
#include<Object3D.h>
#include <SpEffekseer.h>
class GravSphere
{
public:
	GravSphere(const Float3& pos,const Vec3& vel, float speed,
		float gravR, float gravitySpeed, float maxHomeRad, int32_t stayTime);
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

	int32_t stayTime_;
	int32_t timer_ = 0;

	Object3D sphere_;

	Effekseer::Handle hnd_;
};
