#include "stdafx.h"
#include "Marker.h"
#include <Player.h>
#include <SpEffekseer.h>

const TextureKey Marker::TEX_KEY = "Marker";

Marker::Marker()
{
}

void Marker::Cast(const Float3& pos_)
{
    pos = pos_;
    timer_ = 0;
    active = true;
    SpEffekseer::Play("Marker", pos);
}

void Marker::InitModel()
{
    outerObj.model = ModelManager::GetModel("Marker");
    innerObj.model = ModelManager::GetModel("Marker");
}

void Marker::Update()
{
    float scale = (float)R;
    outerObj.scale = { scale, 1, scale };
    outerObj.position = pos;

    float inscale = scale * ((float)(FIN_TIME - timer_) / FIN_TIME);
    innerObj.scale = { inscale, 1, inscale };
    innerObj.position = pos;
    innerObj.position.y += 0.0001f;

    if (timer_ >= FIN_TIME)
    {
        //“–‚½‚è”»’è‚ð‘‚­
        Player* pl = Player::Get();

        Float2 diff = {
            pl->position.x - pos.x,
            pl->position.z - pos.z
        };

        if (abs(diff.x) <= (float)R / 2 + pl->r2d && abs(diff.y) <= (float)R / 2 + pl->r2d)
        {
            pl->Damage();
        }

        active = false;
    }

    outerObj.UpdateMatrix();
    innerObj.UpdateMatrix();

    timer_++;
}

void Marker::Draw()
{
	outerObj.DrawAlpha(TEX_KEY);
	innerObj.DrawAlpha(TEX_KEY);
}
