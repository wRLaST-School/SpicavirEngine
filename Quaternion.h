#pragma once
#include <Vec3.h>
class Quaternion
{
public:
	float w;
	Vec3 v;
public:
	Quaternion();
	Quaternion(Vec3 axis, float rad);
	Quaternion(float real, Vec3 imaginary);
	Quaternion(Vec3 v);

	Quaternion operator* (const Quaternion& o) const;
	Quaternion& operator*= (const Quaternion& o);
	float GetNorm() const;
	Quaternion& Normalize();
	Quaternion& Inverse();
	Matrix GetRotMat();
private:

public:
	static Quaternion Identity();
	static Quaternion Conjugate(const Quaternion& q);
	static Quaternion GetNormalized(const Quaternion& q);
	static Quaternion GetInverse(const Quaternion& q);
};

Vec3 operator *(Vec3 v, Quaternion q);
