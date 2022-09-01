#include "Boss.h"
#include "Util.h"
#include "Player.h"

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
		state = (State)Util::RNG(1, 5, true);
		if((int)state > 4) {
			state = State::Marker;
		}

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
				secondaryState = State::Marker;
				state = (State)Util::RNG(1,3,true);
			}
		}

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

	UpdateBulletsAndMarkers();
}

void Boss::Draw()
{
	*this->brightnessCB.contents = Float4{ 1.0f, 0.25f, 0.25f, 1.0f};
	this->Object3D::Draw("white");
	for (auto& bullet : bullets)
	{
		bullet.Draw();
	}
}

void Boss::DrawMarkers()
{
	for (auto& marker : markers)
	{
		marker.Draw();
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
	UpdateMatrix();
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
	this->rotation.y -= PIf / 40;
	UpdateMatrix();
	if (attackingTimer == 1)
	{
		spreadShotCenter = (Util::RNG(0, 359) * PI / 180);
	}

	if (!(attackingTimer % 6))
	{
		NWay(this->position, 24, 8.0f, spreadShotCenter, 15 * PI / 180);
	}
}

void Boss::AimUpdate()
{
	this->rotation.y -= PIf / 60;
	UpdateMatrix();
	if (!(attackingTimer % 12))
	{
		Vec3 cvel = (Vec3)Player::GetCurrentPlayerPtr()->position - this->position;

		NWay(this->position, pScore->totDamage > pScore->gradeA ? 5 : 3, 6.0f, atan2(cvel.y, cvel.x), PI / 8);
	}
}

void Boss::CircleUpdate()
{
	this->rotation.y += PIf / 30;
	UpdateMatrix();
	if (attackingTimer == 1)
	{
		circleShotCenter = (Util::RNG(0, 359) * PI / 180);
	}
	NWay(this->position, pScore->totDamage > pScore->gradeB ? 2 : 1, 6.0f, circleShotCenter + attackingTimer * 14 * PI / 180, PI);
}

void Boss::MarkerUpdate()
{
	UpdateMatrix();
	if (attackingTimer == 1 || attackingTimer == 121)
	{
		int pattern = Util::RNG(0, 3, true);
		switch (pattern)
		{
		case 0: // rand 10
		{
			for (int i = 0; i < 10; i++)
			{
				markers.emplace_back(Float3{(float)Util::RNG(100, 1100, true), (float)Util::RNG(100, 600), 0.0f});
			}
			break;
		}
		case 1: // aim cross
		{
			Float3 pPos = Marker::WorldToMonitor(Player::GetCurrentPlayerPtr()->position);
			for (int i = 0; i < 13; i++)
			{
				markers.emplace_back(Float3{ (float)i * 100 - 10 + 50, pPos.y, 0});
			}

			for (int i = 0; i < 8; i++)
			{
				markers.emplace_back(Float3{ pPos.x,(float)i * 100 - 40 + 50, 0 });
			}
			break;
		}
		case 2: // line winder
		{
			for (int i = 0; i < 13; i++)
			{
				markers.emplace_back(Float3{ (float)i * 100 - 10 + 50, 160, 0 });

				markers.emplace_back(Float3{ (float)i * 100 - 10 + 50, 720 - 160, 0 });
			}
			break;
		}
		case 3: // edge
		{
			for (int i = 0; i < 13; i++)
			{
				markers.emplace_back(Float3{ (float)i * 100 - 10 + 50, 100, 0 });

				markers.emplace_back(Float3{ (float)i * 100 - 10 + 50, 720 - 100, 0 });
			}

			for (int i = 0; i < 8; i++)
			{
				markers.emplace_back(Float3{ 100,(float)i * 100 - 40 + 50, 0 });
				markers.emplace_back(Float3{ 1280-100,(float)i * 100 - 40 + 50, 0 });
			}
			break;
		}
		default:
			break;
		}
	}
}

void Boss::UpdateBulletsAndMarkers()
{
	for (auto itr = bullets.begin(); itr != bullets.end();)
	{
		itr->Update();

		if (itr->del)
		{
			itr = bullets.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	for (auto itr = markers.begin(); itr != markers.end();)
	{
		if (itr->del)
		{
			itr = markers.erase(itr);
		}
		else
		{
			itr->Update();
			itr++;
		}
	}
}

void Boss::NWay(Float3 pos, int ways, float speed, float centerRad, float radDiff)
{
	for (int i = 0; i < ways; i++)
	{
		Vec3 vel;

		float rad = centerRad - radDiff * ((float)ways - 1)/ 2 + radDiff * i;

		vel = Vec2::RotToVec(rad);

		vel.SetLength(speed);

		bullets.emplace_back(pos, vel, bulletModel);
	}
}

Boss* Boss::pCurrentBoss = nullptr;