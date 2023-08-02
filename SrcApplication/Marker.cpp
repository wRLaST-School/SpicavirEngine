#include "stdafx.h"
#include "Marker.h"
#include <Player.h>
#include <SpEffekseer.h>

const TextureKey Marker::TEX_KEY = "Marker";

Marker::Marker()
{
}

void Marker::Cast(const Float3& pos)
{
    pos_ = pos;
    timer_ = 0;
    active_ = true;
    SpEffekseer::Play("Marker", pos_);
}

void Marker::InitModel()
{
}

void Marker::Update()
{
    //float scale = (float)R;
    //outerObj.scale = { scale, 1, scale };
    //outerObj.position = pos;

    //float inscale = scale * ((float)(FIN_TIME - timer_) / FIN_TIME);
    //innerObj.scale = { inscale, 1, inscale };
    //innerObj.position = pos;
    //innerObj.position.y += 0.0001f;

    if (timer_ >= FIN_TIME)
    {
        //“–‚½‚è”»’è‚ð‘‚­
        Player* pl = Player::Get();

        Float2 diff = {
            pl->position.x - pos_.x,
            pl->position.z - pos_.z
        };

        if (abs(diff.x) <= (float)R / 2 + pl->r2d && abs(diff.y) <= (float)R / 2 + pl->r2d)
        {
            pl->Damage();
        }

        active_ = false;
    }

    //outerObj.UpdateMatrix();
    //innerObj.UpdateMatrix();

    timer_++;
}

void Marker::Draw()
{
	//outerObj.DrawAlpha(TEX_KEY);
	//innerObj.DrawAlpha(TEX_KEY);
}

bool Marker::IsActive()
{
    return active_;
}
