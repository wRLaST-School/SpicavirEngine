#include "stdafx.h"
#include "LineAttack.h"
#include <Player.h>
#include <SpEffekseer.h>

LineAttack::LineAttack(const Float3& pos_, float angle_)
{
    pos = pos_;
    timer_ = 0;
    active = true;
    angle = angle_;
    SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("LineAttack", pos), { 0, 1, 0 }, angle_);

    col.pos = pos_;
    col.rot = Quaternion(Vec3(0, 1, 0), angle_);
    col.scale = { 4.f, 2.f, 20.f };
}

void LineAttack::Update()
{
    if (timer_ >= FIN_TIME)
    {
        Player* pl = Player::Get();

        if (col.Collide(pl->GetCollider()))
        {
            pl->Damage();
        }

        active = false;
    }

    timer_++;
}

void LineAttack::Draw()
{
    col.DrawBB(Color::Red);
}
