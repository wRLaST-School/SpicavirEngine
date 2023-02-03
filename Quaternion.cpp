#include "stdafx.h"
#include "Quaternion.h"
#include <Util.h>

Quaternion::Quaternion()
{
	w = 1.f;
	v = Vec3(0.f, 0.f, 0.f);
}

Quaternion::Quaternion(Vec3 axis, float rad)
{
	w = cos(rad / 2.f);
	v = axis.SetLength(sin(rad / 2.f));
}

Quaternion::Quaternion(float real, Vec3 imaginary)
{
	w = real;
	v = imaginary;
}

Quaternion::Quaternion(Vec3 v)
{
	this->v = v;
	this->w = 0.f;
}

Quaternion Quaternion::operator*(const Quaternion& o) const
{
	return Quaternion(w * o.w - v.Dot(o.v),
		v.Cross(o.v) + (v * o.w) + (o.v * w)
	);
}

Quaternion Quaternion::operator*(const float& o) const
{
	return Quaternion(w * o, v * o);
}

Quaternion& Quaternion::operator*=(const Quaternion& o)
{
	Quaternion t = *this * o;
	*this = t;

	return *this;
}

Quaternion Quaternion::Identity()
{
	return Quaternion();
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return Quaternion(q.w, -q.v);
}

Quaternion Quaternion::GetNormalized(const Quaternion& q)
{
	Quaternion temp = q;
	temp.Normalize();
	return temp;
}

Quaternion Quaternion::GetInverse(const Quaternion& q)
{
	float norm = q.GetNorm();
	norm *= norm;

	Quaternion conj = Conjugate(q);

	return Quaternion(conj.w / norm, Vec3(conj.v.x / norm, conj.v.y / norm, conj.v.z / norm));
}

Quaternion Quaternion::Slerp(const Quaternion& zero, const Quaternion& one, const float& t)
{
	float theta = acos(zero.Dot(one));
	Quaternion zerot = zero * (sin((1.f - t) * theta) / sin(theta));
	Quaternion onet = (one * (sin(t * theta) / sin(theta)));
	Quaternion result;
	
	result.w = zerot.w + onet.w;
	result.v = zerot.v + onet.v;
	return result;
}

Quaternion Quaternion::DirToDir(const Vec3& from, const Vec3& to)
{
	Vec3 f = from;
	Vec3 t = to;

	f.Norm();
	t.Norm();

	float dot = f.Dot(t);

	Vec3 cross = f.Cross(t);

	Vec3 axis = cross.Norm();

	float theta = acos(dot);

	return Quaternion(axis, theta);
}

Quaternion Quaternion::DirToDir(const Vec3& from, const Vec3& to, const float maxRad)
{
	Vec3 f = from;
	Vec3 t = to;

	f.Norm();
	t.Norm();

	float dot = f.Dot(t);

	Vec3 cross = f.Cross(t);

	Vec3 axis = cross.Norm();

	float theta = acos(dot);

	Util::Clamp(theta, 0.f, maxRad);

	return Quaternion(axis, theta);
}

float Quaternion::GetNorm() const
{
	return sqrt(w * w + v.x * v.x + v.y * v.y + v.z * v.z);
}

Quaternion& Quaternion::Normalize()
{
	float norm = GetNorm();
	w /= norm;
	v.x /= norm;
	v.y /= norm;
	v.z /= norm;

	return *this;
}

Quaternion& Quaternion::Inverse()
{
	*this = GetInverse(*this);

	return *this;
}

Matrix Quaternion::GetRotMat()
{
	Vec3 axis = v;
	Float4 X = { axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * w };
	Float3 Y = { axis.y * w, axis.y * axis.y, axis.y * axis.z };
	Float2 Z = { axis.z * axis.z, axis.z * w };
	return Matrix(
		1 - 2 * Y.y - 2 * Z.x,
		2 * X.y + 2 * Z.y,
		2 * X.z - 2 * Y.x,
		0,

		2 * X.y - 2 * Z.y,
		1 - 2 * X.x - 2 * Z.x,
		2 * Y.z + 2 * X.w,
		0,

		2 * X.z + 2 * Y.x,
		2 * Y.z - 2 * X.w,
		1 - 2 * X.x - 2 * Y.y,
		0,

		0, 0, 0, 1
	);
}

float Quaternion::Dot(const Quaternion& o) const
{
	return v.x * o.v.x + v.y * o.v.y + v.z * o.v.z + w * o.w;
}

Vec3 operator*(Vec3 v, Quaternion q)
{
	return (q * Quaternion(v) * Quaternion::Conjugate(q)).v;
}
