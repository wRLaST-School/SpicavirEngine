#pragma once
#include "ICollider.h"
#include "wMath.h"
class SphereCollider :
    public ICollider
{
public:
    SphereCollider(Float3 pos, float r) :pos(pos), r(r) {};

public:
    Float3 pos;
    float r;
};

