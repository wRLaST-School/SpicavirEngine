#include "Player.h"
#include "Input.h"
#include "wSoundManager.h"
#include "Util.h"

void Player::Update()
{
	if(this->health < 3)this->health += 0.075 / 60;

	if (immuneTime > 0) immuneTime--;

	if (state != State::Dodge)
	{
		if(dodgeCD > 0)dodgeCD--;

		if (Input::Key::Down(DIK_X))
		{
			state = State::Slow;
		}
		else
		{
			state = State::Normal;
		}

		if (Input::Key::Down(DIK_LCONTROL) && dodgeCD <= 0)
		{
			state = State::Dodge;
			countered = false;
		}
	}

	void (Player::* PUpdtArray[]) () =
	{
		&Player::NormalUpdate,
		&Player::SlowUpdate,
		&Player::DodgeUpdate
	};

	(this->*PUpdtArray[(int)state])();

	UpdateBullets();
}

void Player::Draw()
{
	if(immuneTime <= 0 || immuneTime % 15 <= 7)
		this->Object3D::Draw();

	for (PlayerBullet& bullet : bullets)
	{
		if (bullet.counter)
		{
			int a = 19037;
		}
		bullet.Draw("notexture");
	}
}

Player* Player::GetCurrentPlayerPtr()
{
	return pPlayer;
}

void Player::SetCurrentPlayer(Player* ptr)
{
	pPlayer = ptr;
}

void Player::Damage()
{
	if (state == State::Dodge && !countered)
	{
		CounterAttack();
		if (this->health < 2.25)
		{
			this->health += 0.75;
		}
		else
		{
			this->health = 3.0f;
		}
		immuneTime = immuneTimeDef;
		return;
	}

	if (immuneTime > 0)
		return;

	this->health -= 1;
	immuneTime = immuneTimeDef;
}

void Player::HealthSprInit()
{
	for (int i = 0; i < 3; i++)
	{
		healthSpr[i] = Sprite("health");

	}

	healthBackSpr = Sprite("healthBack");
}

void Player::DrawHP()
{
	for (int i = 1; i <= health; i++)
	{
		healthSpr[i].position = Float3{ 100.0f + 150 * i, 620.0f, 0.0f};
	}

	int fullhealth = floorf(health);
	float subhealth = health - fullhealth;

	if (fullhealth < 3)
	{
		healthSpr[fullhealth].position = Float3{ 100.0f + 150 * (fullhealth - 1), 620.0f, 0.0f};
		healthSpr->scale = Float3{ subhealth, subhealth, 1 };
	}
}

void Player::NormalUpdate()
{
	NormalMove();
	NormalAttack();
}

void Player::NormalMove()
{
	moveVec =
	{
		/*Input::Pad::GetLStick().x,*/
		(float)Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A),

		/*Input::Pad::GetLStick().y,*/
		(float)Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S),
		0
	};

	if(moveVec.GetSquaredLength() != 0)
		moveVec.SetLength(speed);

	if (moveVec.y)
	{
		this->rotation.x = PI / 8;
	}
	else
	{
		this->rotation.x = 0;
	}

	position += moveVec;

	position.x = Util::Clamp(position.x, -630.0f, 630.0f);
	position.y = Util::Clamp(position.y, -350.0f, 350.0f);

	UpdateMatrix();
}

void Player::SlowUpdate()
{
	SlowMove();
	SlowAttack();
}

void Player::SlowMove()
{
	moveVec =
	{
		/*Input::Pad::GetLStick().x,*/
		(float)Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A),

		/*Input::Pad::GetLStick().y,*/
		(float)Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S),
		0
	};

	if (moveVec.GetSquaredLength() != 0)
		moveVec.SetLength(slowspeed);

	if (moveVec.y)
	{
		this->rotation.x = PI / 8;
	}
	else
	{
		this->rotation.x = 0;
	}

	position += moveVec;

	position.x = Util::Clamp(position.x, -630.0f, 630.0f);
	position.y = Util::Clamp(position.y, -350.0f, 350.0f);

	UpdateMatrix();
}

void Player::UpdateBullets()
{
	for (auto itr = bullets.begin(); itr != bullets.end();)
	{
		itr->Update();

		if (itr->del || itr->timer >= PlayerBullet::lifetime)
		{
			itr = bullets.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Player::NormalAttack()
{
	if (attackCD <= 0)
	{
		if (moveVec.GetSquaredLength() == 0/*Input::Key::Down(DIK_Z)*/)
		{
			bullets.emplace_back((Vec3)this->position + Vec3(0, -15, 0), Vec3(24.0, 0, 0), true);
			bullets.back().model = bulletModel;
			bullets.emplace_back((Vec3)this->position + Vec3(0, 15, 0), Vec3(24.0, 0, 0), true);
			bullets.back().model = bulletModel;
			attackCD = attackCDDef;
			wSoundManager::Play("shot");
		}
	}
	else
	{
		attackCD--;
	}
}

void Player::SlowAttack()
{
	if (attackCD <= 0)
	{
		if (moveVec.GetSquaredLength() == 0/*Input::Key::Down(DIK_Z)*/)
		{
			bullets.emplace_back((Vec3)this->position + Vec3(0, -25, 0), Vec3(24.0, 0, 0), false);
			bullets.back().model = bulletModel;
			bullets.emplace_back((Vec3)this->position + Vec3(0, 25, 0), Vec3(24.0, 0, 0), false);
			bullets.back().model = bulletModel;
			bullets.emplace_back(this->position, Vec3(24.0, 0, 0), false);
			bullets.back().model = bulletModel;
			attackCD = attackCDDef;
			wSoundManager::Play("shot");
		}
	}
	else
	{
		attackCD--;
	}
}

void Player::DodgeUpdate()
{
	dodgeTimer--;
	this->rotation.x = (2 * PI) * ((float)dodgeTimer / dodgeImmuneTime);
	this->position.x += speed * 1.5;
	if (dodgeTimer <= 0)
	{
		state = State::Normal;
		dodgeTimer = dodgeImmuneTime;
		dodgeCD = dodgeCDDef;
	}

	position.x = Util::Clamp(position.x, -630.0f, 630.0f);
	position.y = Util::Clamp(position.y, -350.0f, 350.0f);

	UpdateMatrix();
}

void Player::CounterAttack()
{
	countered = true;

	int counterBulletCounts = 16;
	for (int i = 0; i < counterBulletCounts; i++)
	{
		int deg = Util::RNG(1, 360, true);
		Vec3 velo = Vec2::RotToVec(deg * PI / 180);
		bullets.emplace_back((Vec3)this->position, velo, false, true);
		bullets.back().model = bulletModel;
		bullets.back().damage = 175;
	}
}

Player* Player::pPlayer = nullptr;