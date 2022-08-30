#include "BossBullet.h"
#include "Player.h"

BossBullet::BossBullet(Float3 position, Vec3 velocity):velocity(velocity){
	this->scale = { r, r, r };
	this->position = position;
};

void BossBullet::Update()
{
	Player* player = Player::GetCurrentPlayerPtr();
	{
		position += velocity;
	}

	{
		if (true)
		{
			this->del = true;
			player->Damage();
		}
	}

	UpdateMatrix();

	timer++;
}

