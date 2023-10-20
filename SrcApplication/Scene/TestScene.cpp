#include "TestScene.h"
#include "Input.h"
#include <SpDS.h>

Circle::Circle(Vec2 pos, float r)
{
	pos_ = pos;
	r_ = r;
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

void TestScene::LoadResources()
{
	
}

void TestScene::Init()
{
	Camera::Set(cam);

	circleA = AddComponent<Circle>("Circle", Vec2(400.f, 400.f), 100.f);
	circleB = AddComponent<Circle>("Circle", Vec2(100.f, 100.f), 50.f);
}

void TestScene::Update()
{
	const static float spd = 5.f;

	Vec2 vel;
	vel.x = static_cast<float>(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));
	vel.y = static_cast<float>(Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP));

	if (vel.GetSquaredLength())
	{
		vel.SetLength(spd);
	}

	circleB->pos_ += vel;

	if (circleA->Collide(*circleB))
	{
		circleB->color_ = Color::Blue;
	}
	else
	{
		circleB->color_ = Color::Red;
	}
}

void TestScene::DrawBack()
{

}

void TestScene::Draw3D()
{
	
}

void TestScene::DrawSprite()
{
	circleA->Draw();
	circleB->Draw();
}
