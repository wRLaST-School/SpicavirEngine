#pragma once
#include "ICollider.h"
#include "SpMath.h"
#include <PlaneCollider.h>
class SphereCollider :
    public ICollider
{
public:
    SphereCollider() { pos = {}; r = 0; };
    SphereCollider(Float3 pos, float r) :pos(pos), r(r) {};

    bool Collide(PlaneCollider other);

public:
    Float3 pos;
    float r;
};

