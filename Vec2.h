#pragma once
struct Float2;

class Vec2
{
public:
	float x;
	float y;

	Vec2();

	Vec2(float x, float y);

	void SetX(float x);

	void SetY(float y);

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	void SetLength(float l);

	Vec2 operator+(const Vec2& v2) const;

	Vec2 operator-(const Vec2& v2) const;

	Vec2 operator*(float mul) const;

	Vec2 operator/(float div) const;

	Vec2& operator +=(const Vec2& v2);

	Vec2& operator -=(const Vec2& v2);

	Vec2& operator *=(float mul);

	Vec2& operator /=(float div);

	float Dot(const Vec2& v2) const;

	float Cross(const Vec2& v2) const;

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	Vec2& Norm();

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	Vec2 GetNorm() const;

	float GetLength() const;

	float GetSquaredLength() const;

	operator Float2() const;
};



