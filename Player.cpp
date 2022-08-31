#include "Player.h"
#include "Input.h"
#include "wSoundManager.h"

void Player::Update()
{
	if (state != State::Dodge)
	{
		if (Input::Key::Down(DIK_X))
		{
			state = State::Slow;
		}
		else
		{
			state = State::Normal;
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
	this->Object3D::Draw("notexture");

	for (PlayerBullet& bullet : bullets)
	{
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
	this->health -= 1;
}

void Player::NormalUpdate()
{
	NormalMove();
	NormalAttack();
}

void Player::NormalMove()
{
	Vec3 moveVec =
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

	UpdateMatrix();
}

void Player::SlowUpdate()
{
	SlowMove();
	SlowAttack();
}

void Player::SlowMove()
{
	Vec3 moveVec =
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
		if (Input::Key::Down(DIK_Z))
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
		if (Input::Key::Down(DIK_Z))
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
	if (dodgeTimer <= 0)
	{
		state = Input::Pad::Down(Button::A) ? State::Slow : State::Normal;
		dodgeTimer = dodgeImmuneTime;
	}
}

Player* Player::pPlayer = nullptr;