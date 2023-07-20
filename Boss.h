#pragma once
#include "Object3D.h"
#include <Marker.h>
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

private:
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers;

private:
    static Boss* sCurrent;
};

