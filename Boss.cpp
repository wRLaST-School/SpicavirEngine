#include "stdafx.h"
#include "Boss.h"
#include <Player.h>

void Boss::Update()
{
	totalTimer++;
	for (int i = 0; i < (int)State::Max; i++)
	{
		timer[i] ++;

		attacking[i] = (timer[i] < maxTime[i]) && (timer[i]);
	}

	scale = { 2.0f, 2.0f, 2.0f };

	*brightnessCB.contents = {10.0f, 10.f, 10.f, 1.0f};

	position.y = 3.f + sinf((float)totalTimer / 50.f) / 2.f;

	rotation.y += (PIf / 360);

	UpdateMatrix();

	Light::GetPointLightPtr("BossLight")->pos = this->position;

	if (!attacking[(int)State::Idle])
	{
		timer[(int)State::Idle] = 0;

		Shot(((Vec3)Player::Get()->position - position).SetLength(0.2f));
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
}

void Boss::Shot(Vec3 vel)
{
	bullets.emplace_back(vel, &em[emIndex]);
	emIndex++;
	if (emIndex >= 100) emIndex = 0;
	bullets.back().position = position;
}
