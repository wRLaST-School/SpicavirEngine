#pragma once
#include "ICollider.h"
#include "SphereCollider.h"
#include "wMath.h"
class RayCollider :
    public ICollider
{
public:
    RayCollider(Vec3 ray, Float3 origin = { 0, 0, 0 }) :ray(ray), origin(origin) {};

    bool Collide(const SphereCollider& other)const;
public:
    Vec3 ray;
    Float3 origin;
    bool checkLength = true;
};