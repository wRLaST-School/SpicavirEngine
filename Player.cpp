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
}

void Player::NormalUpdate()
{
	NormalMove();
}

void Player::NormalMove()
{
	Vec3 moveVec =
	{
		Input::Pad::GetLStick().x,
		0,
		Input::Pad::GetLStick().y,
	};

	moveVec.SetLength(speed);

	position += moveVec;

	UpdateMatrix();
}

void Player::SlowUpdate()
{
	SlowMove();
}

void Player::SlowMove()
{
	Vec3 moveVec =
	{
		Input::Pad::GetLStick().x,
		0,
		Input::Pad::GetLStick().y,
	};

	moveVec.SetLength(slowspeed);

	position += moveVec;

	UpdateMatrix();
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
