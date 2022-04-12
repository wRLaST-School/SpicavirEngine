#pragma once
class Vec2
{
public:
	float x;
	float y;

	Vec2();
	Vec2(float x, float y);

	void SetX(float x);

	void SetY(float y);

	//(Sqrtが呼ばれる)
	void SetLength(float l);

	//ベクトルの加算
	const Vec2 operator+(const Vec2& v2) const;

	//ベクトルの減算
	const Vec2 operator-(const Vec2& v2) const;

	//Vecの長さを(xとyそれぞれ)mul倍
	const Vec2 operator*(float mul) const;

	const Vec2 operator/(float div) const;

	//ベクトルの加算
	Vec2& operator +=(const Vec2& v2);

	//ベクトルの減算
	Vec2& operator -=(const Vec2& v2);

	//Vecの長さを(xとyそれぞれ)mul倍
	Vec2& operator *=(float mul);

	Vec2& operator /=(float div);

	//内積を求める
	float Dot(const Vec2& v2) const;
	
	//外積を求める
	float Cross(const Vec2& v2) const;

	//(Sqrtが呼ばれる)
	Vec2& Norm();

	//(Sqrtが呼ばれる)
	Vec2 GetNorm() const;

	float GetLength() const;

	float GetSquaredLength() const;
};

