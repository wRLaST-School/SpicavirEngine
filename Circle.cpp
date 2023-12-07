#include "stdafx.h"
#include "Circle.h"
#include <SpDS.h>
#include <GameManager.h>
CircleCollider::CircleCollider()
{

}

bool CircleCollider::Collide(const CircleCollider& o)
{
	float dist = (pos_ - o.pos_).GetSquaredLength();
	float rsum = powf(r_ + o.r_, 2);
	return dist <= rsum;
}

void CircleCollider::Draw()
{
	if (GameManager::sShowDebug)
	{
		SpDS::DrawCircle(
			static_cast<int32_t>(pos_.x),
			static_cast<int32_t>(pos_.y),
			static_cast<int32_t>(r_),
			color_
		);
	}
}

void CircleCollider::Update()
{
}

void CircleCollider::Init(Vec2 pos, float r)
{
	pos_ = pos;
	r_ = r;
}

void CircleCollider::Init()
{
}
