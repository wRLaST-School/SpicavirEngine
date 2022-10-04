#include "RayCollider.h"

bool RayCollider::Collide(const SphereCollider& other) const
{
    float length = ray.GetLength();

    if (length == 0) return false;

    Vec3 vToSphere = (Vec3)other.pos - this->origin;

    float d = ray.GetNorm(length).Dot(vToSphere);//SUS

    Vec3 rayTemp = ray;

    rayTemp.Norm(length);
    rayTemp *= d;

    Vec3 hLine = (Vec3)other.pos - (rayTemp + this->origin);

    return hLine.GetSquaredLength() <= other.r && d >= 0 && length >= d;
}
