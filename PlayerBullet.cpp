#include "PlayerBullet.h"

void PlayerBullet::Update()
{
	position += velocity;

	UpdateMatrix();

	timer++;
}
