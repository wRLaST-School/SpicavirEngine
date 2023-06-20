#include "stdafx.h"
#include "Player.h"

void Player::Init()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	Object3D::Draw();
}

void Player::Load()
{
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