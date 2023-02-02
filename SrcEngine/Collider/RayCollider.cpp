#include "RayCollider.h"

bool RayCollider::Collide(const SphereCollider& other) const
{
    float length = r.ray.GetLength();

    if (length == 0) return false;

    Vec3 vToSphere = (Vec3)other.pos - this->r.origin;

    float d = r.ray.GetNorm(length).Dot(vToSphere);//SUS

    Vec3 rayTemp = r.ray;

    rayTemp.Norm(length);
    rayTemp *= d;

    Vec3 hLine = (Vec3)other.pos - (rayTemp + this->r.origin);

    return hLine.GetSquaredLength() <= other.r && d >= 0 && length >= d;
}

bool RayCollider::Collide(const PlaneCollider& other, Vec3* intersection) const
{
    //–Ê–@ü‚ÆƒŒƒC•ûŒü‚Ì“àÏ‚ðŒ©‚Ä— –Ê‚©‚ç‚ÌƒŒƒC‚È‚çfalse‚ð•Ô‚·
    float d1 = other.norm.Dot(r.ray);
    if (d1 > -EPSILON) { return false; }

    //“–‚½‚è”»’è‚ðŒ©‚é
    float d2 = other.norm.Dot(r.origin);

    float dist = d2 - other.distance;

    float t = dist / -d1;

    //•‰‚È‚ç“–‚½‚Á‚Ä‚¢‚È‚¢
    if (t < 0) { return false; }

    //“–‚½‚Á‚½‚È‚ç“–‚½‚Á‚Ä‚¢‚éˆÊ’u‚ðŠi”[‚µ‚Ä‚Ù‚µ‚¢‚È‚ç‚·‚é
    if (intersection) { *intersection = r.origin + r.ray * t; }

    return true;
}