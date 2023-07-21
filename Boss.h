#pragma once
#include "Object3D.h"
#include <Marker.h>
#include <LineAttack.h>
class Boss :
    public Object3D
{
public:
    void Init();

    void Update();

    void Draw();

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

private:
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers;

    eastl::list<LineAttack> lineAttacks;

private:
    static Boss* sCurrent;
};

