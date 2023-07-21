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
}

void LineAttack::Update()
{
    if (timer_ >= FIN_TIME)
    {
        //TODO:“–‚½‚è”»’è‚ð‘‚­

  /*      Player* pl = Player::Get();

        Float2 diff = {
            pl->position.x - pos.x,
            pl->position.z - pos.z
        };

        if (abs(diff.x) <= (float)R / 2 + pl->r2d && abs(diff.y) <= (float)R / 2 + pl->r2d)
        {
            pl->Damage();
        }*/

        active = false;
    }

    timer_++;
}

void LineAttack::Draw()
{
    //outerObj.DrawAlpha(TEX_KEY);
    //innerObj.DrawAlpha(TEX_KEY);
}
