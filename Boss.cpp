#include "stdafx.h"
#include "Boss.h"
#include <Player.h>
#include <SoundManager.h>

void Boss::Update()
{
	totalTimer++;
	timer[(int)state] ++;

	scale = { 2.0f, 2.0f, 2.0f };

	*brightnessCB.contents = {10.0f, 10.f, 10.f, 1.0f};

	position.y = 3.f + sinf((float)totalTimer / 50.f) / 2.f;

	rotation.y += (PIf / 360);

	UpdateMatrix();

	Light::GetPointLightPtr("BossLight")->pos = this->position;

	switch (state)
	{
	case State::Idle:
	{
		if (timer[(int)state] >= 60)
		{
			timer[(int)state] = 0;
			state = Util::Chance(50) ? State::Wave: State::Marker;
		}
		break;
	}

	case State::Marker:
	{//jiki
		if (timer[(int)state] % 10 == 0)
		{
			Shot(((Vec3)Player::Get()->position - position).SetLength(0.2f));
		}

		if (timer[(int)state] >= 120)
		{
			timer[(int)state] = 0;
			state = State::Idle;
		}
		break;
	}

	case State::Wave:
	{
		if (timer[(int)state] % 2 == 0)
		{
			Vec3 vel(
				(float)Util::RNG(-1000, 1000),
				(float)Util::RNG(-100, 100),
				(float)Util::RNG(-1000, 1000)
			);
			if (!vel.GetSquaredLength())
			{
				vel = Vec3(0, 0, 1);
			}

			Shot(vel.SetLength(0.2f), true);
		}
		if (timer[(int)state] >= 60)
		{
			timer[(int)state] = 0;
			state = State::Idle;
		}
		break;
	}
	}

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


	for(auto& p : em) p.Update();
}

void Boss::Draw()
{
	Object3D::Draw("white");

	for (auto& b : bullets) b.Draw("white");

	for (auto& p : em) p.Draw();

	if(state == State::Marker)
		LineDrawer::DrawLine(position, Player::Get()->position, { 2.0f, 0.1f, 0.1f, 1.0f });
}

void Boss::Shot(Vec3 vel, bool usePlayerY)
{
	bullets.emplace_back(vel, &em[emIndex]);
	emIndex++;
	if (emIndex >= 100) emIndex = 0;
	bullets.back().position = position;

	if (usePlayerY)
	{
		bullets.back().position.y = Player::Get()->position.y;
	}

	SoundManager::Play("enShot");
}
