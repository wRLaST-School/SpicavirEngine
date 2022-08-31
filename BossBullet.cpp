#include "BossBullet.h"
#include "Player.h"

BossBullet::BossBullet(Float3 position, Vec3 velocity, Model* model):velocity(velocity){
	this->scale = { r, r, r };
	this->position = position;
	this->model = model;
};

void BossBullet::Update()
{
	Player* player = Player::GetCurrentPlayerPtr();
	{
		position += velocity;
		if (position.x * position.x > (640 + r) * (640 + r) || position.y * position.y > (360 + r) * (360 + r))
		{
			del = true;
		}
	}

	{
		if (false)
		{
			this->del = true;
			player->Damage();
		}
	}

	UpdateMatrix();

	timer++;
}

