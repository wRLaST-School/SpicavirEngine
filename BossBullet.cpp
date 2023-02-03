#include "stdafx.h"
#include "BossBullet.h"
#include <Player.h>
#include <Quaternion.h>

void BossBullet::Update()
{
	if (doesHome)
	{
		float l = vel.GetLength();
		Vec3 norm = vel.GetNorm();
		Vec3 plv = (Vec3)Player::Get()->position - position;
		plv.Norm();
		Quaternion rot = Quaternion::DirToDir(norm, plv, maxHomeAngle);
		vel = norm * rot;
		vel *= l;
	}
	
	position += vel;

	*brightnessCB.contents = { 3.0f, 0.0f, 0.0f, 1.0f };

	scale = { 0.5f, .5f, .5f };

	UpdateMatrix();

	totalTimer++;
	if (totalTimer >= 180 || ((Vec3)position).GetSquaredLength() >= 32.f * 32.f) {
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

	em->position = position;
}
