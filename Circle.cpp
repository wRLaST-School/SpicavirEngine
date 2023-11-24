#include "stdafx.h"
#include "Circle.h"
#include <SpDS.h>
Circle::Circle()
{

}

bool Circle::Collide(const Circle& o)
{
	float dist = (pos_ - o.pos_).GetSquaredLength();
	float rsum = powf(r_ + o.r_, 2);
	return dist <= rsum;
}

void Circle::Draw()
{
	SpDS::DrawCircle(
		static_cast<int32_t>(pos_.x),
		static_cast<int32_t>(pos_.y),
		static_cast<int32_t>(r_),
		color_
	);
}

void Circle::Update()
{
}

void Circle::Init(Vec2 pos, float r)
{
	pos_ = pos;
	r_ = r;
}
