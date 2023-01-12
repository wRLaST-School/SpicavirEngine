#pragma once
#include "Object3D.h"
#include <Util.h>
#include <Player.h>
#include <SceneManager.h>
#include <ResultScene.h>
class BossHP :
    public Object3D
{
public:
    BossHP() {
        *brightnessCB.contents = { 0.8f, 0.f, .0f, 1.0f };
        hp = 60 * 60;
    };

    void Update() {
        hp--;

        position.y = 7.f;

        rotation = Player::Get()->rotation;

        scale.x = Util::Clamp((float)hp / 60 /60 * 5.0f, 0.f, 5.f);
        scale.y = 0.25f;
        scale.z = 0.25f;

        UpdateMatrix();

        if (hp <= 0)
        {
            SceneManager::LoadScene<ResultScene>();
        }
    };

    static int hp;
};

