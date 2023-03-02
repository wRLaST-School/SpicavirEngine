#include "stdafx.h"
#include "Player.h"
#include <SpWindow.h>
#include <SpDS.h>
#include <Input.h>

void Player::Update()
{
    Vec2 vel = Vec2(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT),
        Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP));

    if(vel.GetSquaredLength()) vel.SetLength(speed);

    pos += vel;
}

void Player::Draw()
{
    SpDS::DrawRotaGraph(pos.x, pos.y, size.x, size.y, 0.f, tex);
}

void Player::Load()
{
    SpTextureManager::LoadTexture("Resources/candle_player.png", "Player");
}

void Player::Init()
{
    current = Player();
    current.pos.x = GetSpWindow()->width / 2;
    current.pos.y = GetSpWindow()->height / 2;

    current.size = Vec2(0.1f, 0.1f);
}

Player* Player::Get()
{
    return &current;
}

Player Player::current;