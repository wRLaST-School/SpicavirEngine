#include "stdafx.h"
#include "BossBullet.h"

void BossBullet::Update()
{
	position += vel;
	UpdateMatrix();

	totalTimer++;
	if (totalTimer >= 300 || ((Vec3)position).GetSquaredLength() >= 32.f * 32.f) {
		this->del = true;
	}
}
