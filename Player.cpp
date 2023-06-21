#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#include <CameraController.h>
#include <Boss.h>

void Player::Init()
{
	model = ModelManager::GetModel("cube");
	position = { 0, 1, -5 };
}

void Player::Update()
{
	Move();

	if (CameraController::Get()->GetMode() == CameraController::Mode::Target)
	{
		Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		Vec3 to = ((Vec3)Boss::Get()->position - position);
		to.y = 0;
		to.Norm();

		rotation = Quaternion::DirToDir(Vec3(0,0,1), to);
	}

	if (position.y == 1.f && Input::Key::Triggered(DIK_SPACE))
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


	UpdateMatrix();
}

void Player::Move()
{
	Vec3 vel(0, 0, 0);
	Matrix rMat = rotation.GetRotMat();
	Vec3 front = rMat.ExtractAxisZ();
	Vec3 side = rMat.ExtractAxisX();

	vel = front.SetLength((float)(Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)));
	vel += side.SetLength((float)(Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A)));
	vel.y = 0;

	if(vel.GetSquaredLength()) vel.SetLength(spd);

	position += vel;
}

void Player::Draw()
{
	Object3D::Draw("white");
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