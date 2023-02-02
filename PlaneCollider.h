#pragma once
#include "ICollider.h"
class PlaneCollider :
    public ICollider
{
public:
    PlaneCollider() { norm = Vec3(), distance = 0; }
    PlaneCollider(Vec3 norm, float distance) :norm(norm), distance(distance) {};

    Vec3 norm;
    float distance;
};

