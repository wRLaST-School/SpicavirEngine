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

			attackingTimer = 0;
		}
		else
		{
			tAttacks = 0;
			attackTimer = attackTime2;

			attackingTimer = 0;

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

	if (state != State::Idle || secondaryState != State::Idle)
	{
		if (state == State::Idle || attackingTimer >= attackingTimes[(int)state - 1])
		{
			if (secondaryState == State::Idle || attackingTimer >= attackingTimes[(int)secondaryState - 1])
			{
				attackEnd = true;
			}
		}
		attackingTimer++;
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
	for (auto& bullet : bullets)
	{
		bullet.Draw();
	}
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
	if (attackTimer == 60)
	{
		lastPos = position;
		nextPos = {(float)Util::RNG(-4, 4, true) * 128, (float)Util::RNG(-3, 3, true) * 72, 0};
	}
	if (attackTimer <= 60)
	{
		Vec3 v = (Vec3)nextPos - lastPos;
		position = (Vec3)nextPos - (v * (float)attackTimer / 60);
	}
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

void Boss::NWay(Float3 pos, int ways, float speed, float centerRad, float radDiff)
{
	for (int i = 0; i < ways; i++)
	{
		Vec3 vel;

		float rad = centerRad - radDiff * (float)ways / 2 + radDiff * i;

		vel = Vec2::RotToVec(rad);

		vel.SetLength(speed);

		bullets.emplace_back(pos, vel);
	}
}

Boss* Boss::pCurrentBoss = nullptr;