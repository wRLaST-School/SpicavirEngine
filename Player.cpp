#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#include <CameraController.h>

void Player::Init()
{
	model = ModelManager::GetModel("cube");
}

void Player::Update()
{
	Move();

	if (CameraController::Get()->GetMode() == CameraController::Mode::Target)
	{

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