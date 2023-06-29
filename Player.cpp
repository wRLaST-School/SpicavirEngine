#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#include <CameraController.h>
#include <Boss.h>

void Player::Init()
{
	model = ModelManager::GetModel("cube");
	position = { 0, 1, -5 };
	scale = { 0.5, 1.0, 0.5 };
}

void Player::Update()
{
	Move();

	if (Input::Key::Triggered(DIK_R) || Input::Pad::Triggered(Button::R))
	{
		CameraController::Get()->ToggleMode();
	}
	if (CameraController::Get()->GetMode() == CameraController::Mode::Target)
	{
		Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		Vec3 to = ((Vec3)Boss::Get()->position - position);
		to.y = 0;
		to.Norm();

		rotation = Quaternion::DirToDir(Vec3(0, 0, 1), to);
	}
	else
	{
		//ƒtƒŠ[ƒJƒƒ‰Žž‚Ìˆ—
	}
	DamageUpdate();
	
	switch (state)
	{
	case Player::State::Idle:
	case Player::State::Move:
		IdleMoveUpdate();
		break;
	case Player::State::Dodge:
		DodgeUpdate();
		break;
	default:
		break;
	}
	
	UpdateMatrix();
}

void Player::Move()
{
	Vec3 vel(0, 0, 0);
	//Matrix rMat = rotation.GetRotMat();
	CameraController* ctrl = CameraController::Get();
	Vec3 front = (Vec3)ctrl->cam->target - ctrl->cam->position;
	Vec3 side = front.Cross(Vec3(0.f, 1.f, 0.f));

	vel = front.SetLength((float)(Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)));
	vel += -side.SetLength((float)(Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A)));

	front = (Vec3)ctrl->cam->target - ctrl->cam->position;
	side = front.Cross(Vec3(0.f, 1.f, 0.f));

	vel += -side.SetLength(Input::Pad::GetLStick().x);
	vel += front.SetLength(Input::Pad::GetLStick().y);

	vel.y = 0;

	if(vel.GetSquaredLength()) vel *= spd;

	position += vel;
}

void Player::DamageUpdate()
{
	damageTimer--;
	damageTimer = max(0, damageTimer);

	if (damageTimer)
	{
		*brightnessCB.contents = { 1.f, 0.f, 0.f, .5f };
	}
	else
	{
		*brightnessCB.contents = { 1.f, 1.f, 1.f, 1.f };
	}
}

void Player::Damage()
{
	damageTimer = 60;
}

void Player::Draw()
{
	Object3D::DrawAlpha("white");
}

void Player::DodgeUpdate()
{
	position += dodgeVec_;

	dodgeTimer++;

	if (dodgeTimer >= iFrame)
	{
		state = State::Idle;
	}
}

void Player::IdleMoveUpdate()
{


	if (position.y == 1.f && (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A)))
	{
		vy = JUMP_POWER;
	}
	if (position.y > 1.f)
	{
		vy -= GRAV;
	}

	position.y += vy;

	if (position.y < 1.f)
	{
		vy = 0;
		position.y = 1.f;
	}
}

void Player::Load()
{
	ModelManager::Register("Cube", "cube");
}

Player* Player::Get()
{
	return sCurrent;
}

void Player::Set(Player* player)
{
	sCurrent = player;
}

Player* Player::sCurrent = nullptr;