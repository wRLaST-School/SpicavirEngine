#include "Boss.h"
#include "Util.h"

Boss::Boss(Score* score)
{
	position = { 0,0,0 };
	scale = { 100, 100, 100 };
	this->pScore = score;
}

void Boss::Update()
{
	if (attackTimer <= 0)
	{
		if (tAttacks < 2 || (tAttacks < 3 && pScore->totDamage > pScore->gradeA))
		{
			tAttacks++;
			attackTimer = attackTime1;
		}
		else
		{
			tAttacks = 0;
			attackTimer = attackTime2;

			if (pScore->totDamage > pScore->gradeA)
			{
				secondaryState = (State)Util::RNG(1, 4, true);
			}
		}

		state = (State)Util::RNG(1, 4, true);
	}

	if (state != State::Idle && attackEnd)
	{
		attackEnd = false;
		state = State::Idle;
		secondaryState = State::Idle;
	}

	void (Boss:: * PUpdtArray[]) () =
	{
		&Boss::IdleUpdate,
		&Boss::SpreadUpdate,
		&Boss::AimUpdate,
		&Boss::CircleUpdate,
		&Boss::MarkerUpdate
	};

	(this->*PUpdtArray[(int)state])();

	if (secondaryState != State::Idle)
	{
		(this->*PUpdtArray[(int)secondaryState])();
	}
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
	this->attackTimer--;
	UpdateMatrix();
}

void Boss::SpreadUpdate()
{
}

void Boss::AimUpdate()
{
}

void Boss::CircleUpdate()
{
}

void Boss::MarkerUpdate()
{
}

Boss* Boss::pCurrentBoss = nullptr;