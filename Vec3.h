#pragma once

struct Float3;
class Vec2;

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float x, float y, float z);

	//Sqrt���Ă΂��
	Vec3& SetLength(float l);

	Vec3 operator+(const Vec3& v2) const;

	Vec3 operator-(const Vec3& v2) const;

	Vec3 operator*(float mul) const;

	Vec3 operator/(float div) const;

	Vec3& operator +=(const Vec3& v2);

	Vec3& operator -=(const Vec3& v2);

	Vec3& operator *=(float mul);

	Vec3& operator /=(float div);

	bool operator > (const Vec3& v2) const;
	bool operator >= (const Vec3& v2) const;
	bool operator < (const Vec3& v2) const;
	bool operator <= (const Vec3& v2) const;

	//Sqrt���Ă΂��
	Vec3& Norm();
	//Sqrt���Ă΂�Ȃ��B�������킩���Ă���ꍇ�ɁB
	Vec3& Norm(float length);

	//Sqrt���Ă΂��
	Vec3 GetNorm() const;
	//Sqrt���Ă΂�Ȃ��B�������킩���Ă���ꍇ�ɁB
	Vec3 GetNorm(float length) const;

	float GetLength() const;

	float GetSquaredLength() const;

	float Dot(const Vec3& v2) const;
	Vec3 Cross(const Vec3& v2) const;

	operator Float3() const;
	operator Vec2() const;
};

