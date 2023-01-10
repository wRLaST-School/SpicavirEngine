#include "stdafx.h"
#include "BossBullet.h"
#include <Player.h>

void BossBullet::Update()
{
	position += vel;
	UpdateMatrix();

	totalTimer++;
	if (totalTimer >= 300 || ((Vec3)position).GetSquaredLength() >= 32.f * 32.f) {
		this->del = true;
	}

	Player* pl = Player::Get();
	float dif = ((Vec3)pl->position - position).GetSquaredLength();
	float sumR = powf(pl->scale.x + scale.x, 2);
	if (
		dif < sumR
		)
	{
		this->del = true;
		pl->Damage();
	}
}
