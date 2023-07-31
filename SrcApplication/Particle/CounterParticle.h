#pragma once
#include "IParticle.h"
class CounterParticle :
    public IParticle
{
public:
    CounterParticle(Float3 pos);

    void Update();

    static TextureKey sTexture;
    int32_t maxTime = 90;

    Vec3 firstPos;
    Vec3 secondPos;

    float speed = 0.15f;
    float speedDec = speed / maxTime;

    float spread = 5.f;
};

