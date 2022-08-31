#include "Marker.h"
#include "wWindow.h"
#include "Player.h"

Float3 Marker::MonitorToWorld(Float3 monitor)
{
    return { monitor.x - GetwWindow()->width / 2, (monitor.y - GetwWindow()->height / 2) * -1, 0.f };
}

Float3 Marker::WorldToMonitor(Float3 world)
{
    return { world.x + GetwWindow()->width / 2, (-1 * world.y + GetwWindow()->height / 2), 0.f };
}

void Marker::SetMarkerTextures(TextureKey outer, TextureKey inner)
{
    outertex = outer;
    innertex = inner;
}

Marker::Marker(Float3 moniPos):pos(moniPos)
{
    outerSpr = Sprite(outertex);
    innerSpr = Sprite(innertex);
}

void Marker::Update()
{
    float scale = (float)r / spriteR;
    outerSpr.scale = {scale, scale, 1};
    outerSpr.position = pos;

    float inscale = scale * ((float)(finTime - timer) / finTime);
    innerSpr.scale = {inscale, inscale, 1};
    innerSpr.position = pos;

    if (timer >= finTime)
    {
        outerSpr.brightness = {10000, 10000, 10000};

        Player* player = Player::GetCurrentPlayerPtr();
        Float3 csprw = MonitorToWorld(pos);

        Float2 diff = {
            player->position.x - csprw.x,
            player->position.y - csprw.y
        };

        if (abs(diff.x) <= r + player->r && abs(diff.y) <= r + player->r)
        {
            player->Damage();
        }

        this->del = true;
    }
    
    outerSpr.UpdateMatrix();
    innerSpr.UpdateMatrix();

    timer++;
}

void Marker::Draw()
{
    outerSpr.Draw();
    innerSpr.Draw();
}

TextureKey Marker::outertex;
TextureKey Marker::innertex;