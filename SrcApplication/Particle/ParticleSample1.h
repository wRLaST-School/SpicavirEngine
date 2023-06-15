#pragma once
#include "IParticle.h"
#include <Util.h>
class ParticleSample1 :
    public IParticle
{
public:
    ParticleSample1(Float3 pos) : IParticle(pos) {
        vel.x = (float)Util::RNG(0, 32767) / 32767;
        vel.y = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
        vel.z = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
    };

    void Update();

    static TextureKey texture;
    int32_t maxTime = 120;

    Vec3 vel;

    float speed = 0.15f;
    float speedDec = speed / maxTime;
};

