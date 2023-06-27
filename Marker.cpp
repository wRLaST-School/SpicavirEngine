#include "stdafx.h"
#include "Marker.h"

Marker::Marker()
{
}

void Marker::Cast(Float3 pos_)
{
    pos = pos_;
    timer_ = 0;
    active = true;
}

void Marker::Update()
{
    float scale = (float)R;
    outerObj.scale = { scale, scale, 1 };
    outerObj.position = pos;

    float inscale = scale * ((float)(FIN_TIME - timer_) / FIN_TIME);
    innerObj.scale = { inscale, inscale, 1 };
    innerObj.position = pos;

    if (timer_ >= FIN_TIME)
    {
        //“–‚½‚è”»’è‚ğ‘‚­

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
