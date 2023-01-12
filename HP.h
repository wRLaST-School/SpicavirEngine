#pragma once
#include "Object3D.h"
#include <Player.h>
class HP :
    public Object3D
{
public:
    HP() {
        *brightnessCB.contents = {0.f, 0.8f, .8f, 1.0f};
        hp = 10;
    };

    void Update() {
        hp = Player::Get()->health;

        position = Player::Get()->position;
        position.y += 1.5f;

        rotation = Player::Get()->rotation;

        scale.x = Util::Clamp((float)hp / 8, 0.f, 2.5f);
        scale.y = 0.125f;
        scale.z = 0.125f;

        UpdateMatrix();
    };

    static int hp;
};