#include "Boss.h"

Boss::Boss(Score* score)
{
	position = { 0,0,0 };
	scale = { 100, 100, 100 };
	this->pScore = score;
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
	this->pScore->totDamage += damage;
}
