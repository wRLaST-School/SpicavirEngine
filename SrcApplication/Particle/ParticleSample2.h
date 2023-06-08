#pragma once
#include "IParticle.h"
#include <Util.h>
class ParticleSample2 :
    public IParticle
{
public:
    ParticleSample2(Float3 pos) : IParticle(pos) {
        vel.x = (float)Util::RNG(0, 8192) / 32767 * (Util::Chance(50) ? 1 : -1);
        vel.y = (float)Util::RNG(0, 32767) / 32767 * (-1);
        vel.z = (float)Util::RNG(0, 8192) / 32767 * (Util::Chance(50) ? 1 : -1);
    };

    void Update();

    static TextureKey texture;
    int32_t maxTime = 120;

    Vec3 vel;

    float speed = 0;
    float speedInc = 0.15f / maxTime;
};