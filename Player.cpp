#include "Player.h"
#include "Input.h"

void Player::Update()
{
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

void Player::NormalUpdate()
{
	NormalMove();
	AttackCommon();
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

	position += moveVec;

	UpdateMatrix();
}

void Player::SlowUpdate()
{
	SlowMove();
	AttackCommon();
}

void Player::SlowMove()
{
	Vec3 moveVec =
	{
		/*Input::Pad::GetLStick().x,*/
		(float)Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A),
		0,
		/*Input::Pad::GetLStick().y,*/
		(float)Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)
	};

	moveVec.SetLength(slowspeed);

	position += moveVec;

	UpdateMatrix();
}

void Player::UpdateBullets()
{
	for (auto itr = bullets.begin(); itr != bullets.end();)
	{
		itr->Update();

		if (itr->timer >= PlayerBullet::lifetime)
		{
			itr = bullets.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Player::AttackCommon()
{
	if (attackCD <= 0)
	{
		if (Input::Key::Down(DIK_Z))
		{
			bullets.emplace_back(this->position, Vec3(24.0, 0, 0));
			bullets.back().model = model;
			attackCD = attackCDDef;
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
