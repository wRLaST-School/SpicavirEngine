#pragma once
#include "Object3D.h"
#include <Marker.h>
#include <LineAttack.h>
#include <OBBCollider.h>
class Boss :
    public Object3D
{
public:
    void Init();

    void Update();

    void Draw();
    
    void Damage();

public:
    static void Load();

    static Boss* Get();

    static void Set(Boss* boss);

public:
    void CastMarker(Float3 pos);

    void CastMarkerAim1Rand5();
    void CastMarkerLine3();

    void DrawMarkers();
    void UpdateMarkers();

    void CastLineTriple();
    void CastLine(Float3 pos, float angle);

    void UpdateLineAttacks();
    void DrawLineAttacks();

    const OBBCollider& GetCollider();

private:
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers;

    eastl::list<LineAttack> lineAttacks;

    float markerLine3Spacing = 20.f;
    float lineAttackSpacing = 15.f;

    OBBCollider col;

    int32_t damageTimer = 0;
    const int32_t damageTime = 15;
    bool damaged = false;

private:
    static Boss* sCurrent;
};

