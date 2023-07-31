#pragma once
#include "IParticle.h"
class CounterParticle :
    public IParticle
{
public:
    CounterParticle(Float3 pos);

    void Update();

    static TextureKey sTexture;
    int32_t maxTime_ = 90;

    Vec3 firstPos_;
    Vec3 secondPos_;

    float speed_ = 0.15f;
    float speedDec_ = speed_ / maxTime_;

    float spread_ = 5.f;
};

