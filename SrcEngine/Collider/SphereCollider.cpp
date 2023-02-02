#include "SphereCollider.h"

bool SphereCollider::Collide(PlaneCollider other)
{
    float oDist = ((Vec3)this->pos).Dot(other.norm);
    float aDist = oDist - other.distance;
    if (aDist < 0) aDist = -aDist;

    return aDist <= this->r;
}
