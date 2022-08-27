#include "PlayerBullet.h"
#include "Boss.h"

void PlayerBullet::Update()
{
	if (isHoming)
	{
		Boss* boss = Boss::GetCurrentBossPtr();

		Vec3 homeVel = (Vec3)boss->position - this->position;
		homeVel.SetLength(velocity.GetLength());
		position += homeVel;
	}
	else
	{
		position += velocity;
	}

	UpdateMatrix();

	timer++;
}
