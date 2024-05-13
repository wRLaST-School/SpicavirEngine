#pragma once
#include "ICollider.h"
class DLLExport PlaneCollider :
    public ICollider
{
public:
    PlaneCollider(Vec3 norm, float distance) :norm(norm), distance(distance) {};

    Vec3 norm;
    float distance;
};

