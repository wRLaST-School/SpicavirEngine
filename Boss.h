#pragma once
#include "Object3D.h"
#include <Light.h>
#include <BossBullet.h>
class Boss :
    public Object3D
{
public:
    Boss() {
        Light::CreatePointLight(position, {3.f, 3.f, 3.f}, {0.1f, 0.1f, 0.01f}, "BossLight");
        for (auto& p : em) p.active = false;
    };
    void Update();

    void Draw();

    void Shot(Vec3 vel, bool usePlayerY = false);

    int health;
    static const int maxhealth = 100;

    enum class State {
        Idle,
        Wave,
        Marker,
        Line,
        Max
    } state = State::Idle;

    bool attacking[(int)State::Max] = {};

    int timer[(int)State::Max] = {};

    int maxTime[(int)State::Max] = {
        60,
        60,
        60,
        60
    };

    int totalTimer = 0;

    list<BossBullet> bullets;

    Emitter<ParticleSample1> em[100];
    int emIndex = 0;
};

