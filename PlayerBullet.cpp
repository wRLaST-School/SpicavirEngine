#include "PlayerBullet.h"
#include "Boss.h"

PlayerBullet::PlayerBullet(Float3 position, Vec3 velocity, bool isHoming, bool counter) :velocity(velocity), isHoming(isHoming), counter(counter) {
	this->scale = { r, r, r };
	this->position = position;
};

void PlayerBullet::Update()
{
	Boss* boss = Boss::GetCurrentBossPtr();
	if (counter)
	{
		if (timer > 20)
		{
			Vec3 homeVel = (Vec3)boss->position - this->position;
			homeVel.SetLength(counterSpeed);
			position += homeVel;

			counterSpeed++;
			counterSpeed = min(counterSpeed, 24);
		}
		else
		{
			velocity.SetLength(counterSpeed == 0 ? 1 : counterSpeed);
			position += velocity;
			counterSpeed--;
		}
	}
	else
	{
		if (isHoming)
		{
			Vec3 homeVel = (Vec3)boss->position - this->position;
			homeVel.SetLength(velocity.GetLength());
			position += homeVel;
		}
		else
		{
			position += velocity;
		}
	}

	{
		if ((boss->position.y - this->position.y) * (boss->position.y - this->position.y) < (r + boss->scale.y) * (r + boss->scale.y)
			&& (boss->position.x - this->position.x) * (boss->position.x - this->position.x) < ((r + velocity.GetLength() / 2) * (r + velocity.GetLength() / 2)))
		{
			this->del = true;
			boss->Damage(damage);
		}
	}

	UpdateMatrix();

	timer++;
}
