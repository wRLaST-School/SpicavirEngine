#pragma once
#include "ICollider.h"
#include "SphereCollider.h"
#include "SpMath.h"
class RayCollider :
    public ICollider
{
public:
    RayCollider() { r = Ray(); checkLength = false; }
    RayCollider(Vec3 ray, Float3 origin = { 0, 0, 0 }) :r(ray, origin){};
    RayCollider(const Ray& r) : r(r) {};

    bool Collide(const SphereCollider& other)const;
    bool Collide(const PlaneCollider& other, Vec3* intersectionOut = nullptr) const;
public:
    Ray r;
    bool checkLength = true;
};