#pragma once
#include "IParticle.h"
#include <Util.h>
class ParticleSample1 :
    public IParticle
{
public:
    ParticleSample1(Float3 pos) : IParticle(pos) {

        maxTime = 60.f;
    };

    void Update();

    static TextureKey texture;
};

