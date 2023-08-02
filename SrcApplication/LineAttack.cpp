#include "stdafx.h"
#include "LineAttack.h"
#include <Player.h>
#include <SpEffekseer.h>

LineAttack::LineAttack(const Float3& pos_, float angle_)
{
    pos = pos_;
    timer_ = 0;
    active_ = true;
    angle_ = angle_;
    SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("LineAttack", pos), { 0, 1, 0 }, angle_);

    col_.pos = pos_;
    col_.rot = Quaternion(Vec3(0, 1, 0), angle_);
    col_.scale = { 4.f, 2.f, 20.f };
}

void LineAttack::Update()
{
    if (timer_ >= FIN_TIME)
    {
        Player* pl = Player::Get();

        if (col_.Collide(pl->GetCollider()))
        {
            pl->Damage();
        }

        active_ = false;
    }

    timer_++;
}

void LineAttack::Draw()
{
    col_.DrawBB(Color::Red);
}

bool LineAttack::IsActive()
{
    return active_;
}
