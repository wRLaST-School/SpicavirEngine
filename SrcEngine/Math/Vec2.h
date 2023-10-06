#pragma once
struct Float2;
class Vec3;

class Vec2
{
public:
	float x;
	float y;

	Vec2();

	Vec2(float x, float y);

	void SetX(float x);

	void SetY(float y);

	//Sqrtが呼ばれる
	void SetLength(float l);

	Vec2 operator+(const Vec2& v2) const;

	Vec2 operator-(const Vec2& v2) const;

	Vec2 operator*(float mul) const;

	Vec2 operator/(float div) const;

	Vec2& operator +=(const Vec2& v2);

	Vec2& operator -=(const Vec2& v2);

	Vec2& operator *=(float mul);

	Vec2& operator /=(float div);

	bool operator > (const Vec2& v2) const;
	bool operator >= (const Vec2& v2) const;
	bool operator < (const Vec2& v2) const;
	bool operator <= (const Vec2& v2) const;

	float Dot(const Vec2& v2) const;

	float Cross(const Vec2& v2) const;

	//Sqrtが呼ばれる
	Vec2& Norm();

	//Sqrtが呼ばれる
	Vec2 GetNorm() const;

	float GetLength() const;

	float GetSquaredLength() const;

	static float VecToVec(Vec2 from, Vec2 to);

	operator Float2() const;
	operator Vec3() const;
};



