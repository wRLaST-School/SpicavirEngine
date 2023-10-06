#include "stdafx.h"
#include "OBBCollider.h"
#include <LineDrawer.h>
#include <GameManager.h>

//sepは正規化されていること
float GetSeparateAxisLength(const Vec3& sep, const Vec3& e1, const Vec3& e2, const Vec3& e3 = Vec3(0, 0, 0))
{
    //3つの内積の絶対値の合計が投影線分の長さ
    float r1 = fabsf(sep.Dot(e1));
    float r2 = fabsf(sep.Dot(e2));
    float r3 = e3.GetSquaredLength() ? fabsf(sep.Dot(e3)) : 0;

    return r1 + r2 + r3;
}

bool OBBCollider::Collide(const OBBCollider& other)
{
    Matrix aRotMat = rot.GetRotMat();

    Vec3 axVecNorm = aRotMat.ExtractAxisX().GetNorm();
    Vec3 ayVecNorm = aRotMat.ExtractAxisY().GetNorm();
    Vec3 azVecNorm = aRotMat.ExtractAxisZ().GetNorm();

    Vec3 axVec = axVecNorm * scale.x;
    Vec3 ayVec = ayVecNorm * scale.y;
    Vec3 azVec = azVecNorm * scale.z;

    Matrix bRotMat = other.rot.GetRotMat();

    Vec3 bxVecNorm = bRotMat.ExtractAxisX().GetNorm();
    Vec3 byVecNorm = bRotMat.ExtractAxisY().GetNorm();
    Vec3 bzVecNorm = bRotMat.ExtractAxisZ().GetNorm();

    Vec3 bxVec = bxVecNorm * other.scale.x;
    Vec3 byVec = byVecNorm * other.scale.y;
    Vec3 bzVec = bzVecNorm * other.scale.z;

    Vec3 interval = (Vec3)pos - other.pos;

    //分離軸Ae1
    float rA = scale.x;
    float rB = GetSeparateAxisLength(axVecNorm, bxVec, byVec, bzVec);
    float l = fabsf(interval.Dot(axVecNorm));

    if (l > rA + rB) return false;

    //分離軸Ae2

    rA = scale.y;
    rB = GetSeparateAxisLength(ayVecNorm, bxVec, byVec, bzVec);
    l = fabsf(interval.Dot(ayVecNorm));

    if (l > rA + rB) return false;

    //分離軸Ae3

    rA = scale.z;
    rB = GetSeparateAxisLength(azVecNorm, bxVec, byVec, bzVec);
    l = fabsf(interval.Dot(azVecNorm));

    if (l > rA + rB) return false;

    //分離軸Be1

    rA = GetSeparateAxisLength(bxVecNorm, axVec, ayVec, azVec);
    rB = other.scale.x;
    l = fabsf(interval.Dot(bxVecNorm));

    if (l > rA + rB) return false;

    //分離軸Be2

    rA = GetSeparateAxisLength(byVecNorm, axVec, ayVec, azVec);
    rB = other.scale.y;
    l = fabsf(interval.Dot(byVecNorm));

    if (l > rA + rB) return false;

    //分離軸Be3

    rA = GetSeparateAxisLength(bzVecNorm, axVec, ayVec, azVec);
    rB = other.scale.z;
    l = fabsf(interval.Dot(bzVecNorm));

    if (l > rA + rB) return false;

    //分離軸ax bx
    Vec3 cross = axVecNorm.Cross(bxVecNorm);
    rA = GetSeparateAxisLength(cross, ayVec, azVec);
    rB = GetSeparateAxisLength(cross, byVec, bzVec);
    l = fabsf(interval.Dot(cross));
    
    if (l > rA + rB) return false;

    //分離軸ax by
    cross = axVecNorm.Cross(byVecNorm);
    rA = GetSeparateAxisLength(cross, ayVec, azVec);
    rB = GetSeparateAxisLength(cross, bxVec, bzVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸ax bz
    cross = axVecNorm.Cross(bzVecNorm);
    rA = GetSeparateAxisLength(cross, ayVec, azVec);
    rB = GetSeparateAxisLength(cross, bxVec, byVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸ay bx
    cross = ayVecNorm.Cross(bxVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, azVec);
    rB = GetSeparateAxisLength(cross, byVec, bzVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸ay by
    cross = ayVecNorm.Cross(byVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, azVec);
    rB = GetSeparateAxisLength(cross, bxVec, bzVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸ay bz
    cross = ayVecNorm.Cross(bzVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, azVec);
    rB = GetSeparateAxisLength(cross, bxVec, byVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸az bx
    cross = azVecNorm.Cross(bxVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, ayVec);
    rB = GetSeparateAxisLength(cross, byVec, bzVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸az by
    cross = azVecNorm.Cross(byVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, ayVec);
    rB = GetSeparateAxisLength(cross, bxVec, bzVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離軸az bz
    cross = azVecNorm.Cross(bzVecNorm);
    rA = GetSeparateAxisLength(cross, axVec, ayVec);
    rB = GetSeparateAxisLength(cross, bxVec, byVec);
    l = fabsf(interval.Dot(cross));

    if (l > rA + rB) return false;

    //分離できないため当たっている
    return true;
}

void OBBCollider::DrawBB(Color color)
{
    if (GameManager::sShowHitBoxes)
    {
        LineDrawer::DrawRotaCube(pos, scale, rot, color.f4);
    }
}
