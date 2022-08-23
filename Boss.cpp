#include "Boss.h"

Boss::Boss()
{
	position = { 0,0,0 };
	scale = { 100, 100, 100 };
}

void Boss::Update()
{
	UpdateMatrix();
}

void Boss::Draw()
{
	this->Object3D::Draw();
}

void Boss::Damage(int damage)
{
	this->damage += damage;
}
