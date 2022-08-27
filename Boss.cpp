#include "Boss.h"

Boss::Boss(Score* score)
{
	position = { 0,0,0 };
	scale = { 100, 100, 100 };
	this->pScore = score;
}

void Boss::Update()
{
	void (Boss:: * PUpdtArray[]) () =
	{
		&Boss::IdleUpdate,
	};

	(this->*PUpdtArray[(int)state])();
}

void Boss::Draw()
{
	this->Object3D::Draw();
}

void Boss::Damage(int damage)
{
	this->pScore->totDamage += damage;
}

Boss* Boss::GetCurrentBossPtr()
{
	return pCurrentBoss;
}

void Boss::SetCurrentBoss(Boss* ptr)
{
	pCurrentBoss = ptr;
}

void Boss::IdleUpdate()
{
	this->rotation.y += PIf / 60;
	UpdateMatrix();
}

Boss* Boss::pCurrentBoss = nullptr;