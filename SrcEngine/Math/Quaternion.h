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
	Quaternion operator* (const float& o) const;
	Quaternion& operator*= (const Quaternion& o);
	bool operator == (const Quaternion& o) const;

	float GetNorm() const;
	Quaternion& Normalize();
	Quaternion& Inverse();
	Matrix GetRotMat();

	float Dot(const Quaternion& o) const;
private:

public:
	static Quaternion Identity();
	static Quaternion Conjugate(const Quaternion& q);
	static Quaternion GetNormalized(const Quaternion& q);
	static Quaternion GetInverse(const Quaternion& q);
	static Quaternion Slerp(const Quaternion& zero, const Quaternion& one, const float& t);
	static Quaternion DirToDir(const Vec3& from, const Vec3& to);
	static Quaternion DirToDir(const Vec3& from, const Vec3& to, const float maxRad);
};

Vec3 operator *(Vec3 v, Quaternion q);
