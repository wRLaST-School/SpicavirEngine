#include "Vec2.h"
#include <math.h>
#include "Float2.h"

Vec2::Vec2() :x(0), y(0)
{
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

void Vec2::SetX(float x)
{
	this->x = x;
}

void Vec2::SetY(float y)
{
	this->y = y;
}

void Vec2::SetLength(float l)
{
	float len2 = this->GetLength();
	float mul = l / len2;
	x *= mul;
	y *= mul;
}

Vec2 Vec2::operator+(const Vec2& v2) const
{
	return Vec2(this->x + v2.x, this->y + v2.y);
}

Vec2 Vec2::operator-(const Vec2& v2) const
{
	return Vec2(this->x + v2.x, this->y + v2.y);
}

Vec2 Vec2::operator*(float mul) const
{
	return Vec2(this->x * mul, this->y * mul);
}

Vec2 Vec2::operator/(float div) const
{
	return Vec2(this->x / div, this->y / div);
}

Vec2& Vec2::operator+=(const Vec2& v2)
{
	this->x += v2.x;
	this->y += v2.y;

	return *this;
}

Vec2& Vec2::operator-=(const Vec2& v2)
{
	this->x -= v2.x;
	this->y -= v2.y;

	return *this;
}

Vec2& Vec2::operator*=(float mul)
{
	this->x *= mul;
	this->y *= mul;

	return *this;
}

Vec2& Vec2::operator/=(float div)
{
	this->x /= div;
	this->y /= div;

	return *this;
}

float Vec2::Dot(const Vec2& v2) const
{
	return (this->x * v2.x) + (this->y * v2.y);
}

float Vec2::Cross(const Vec2& v2) const
{
	return (this->x * v2.y) - (this->y * v2.x);
}

Vec2& Vec2::Norm()
{
	float l = this->GetLength();
	if (l) {
		this->x /= l;
		this->y /= l;
	}
	else {
		this->x = 0;
		this->y = 0;
	}
	return *this;
}

Vec2 Vec2::GetNorm() const
{
	float l = this->GetLength();
	if (l)
		return Vec2(this->x / l, this->y / l);
	else
		return Vec2();
}

float Vec2::GetLength() const
{
	return sqrtf(this->GetSquaredLength());
}

float Vec2::GetSquaredLength() const
{
	return this->x * this->x + this->y * this->y;
}

Vec2::operator Float2() const
{
	return Float2{ x, y };
}
