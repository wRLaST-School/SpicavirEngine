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

    void Rush();
    void RushUpdate();
    void RushEnd();

    void SelectMove();

    const OBBCollider& GetCollider();

private:
    //Marker
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers;

    //LineAttack
    eastl::list<LineAttack> lineAttacks;

    float markerLine3Spacing = 20.f;
    float lineAttackSpacing = 15.f;

    //Rush
    int32_t prepTime = 30;
    int32_t afterPrepWaitTime = 30;
    int32_t rushTime = 60;
    int32_t rushAfterTime = 30;
    float rushDistance = 20.f;
    Float3 rushTarget = {};

    bool dealDamageOnHit = false;

    //Collision
    OBBCollider col;

    //Damage
    int32_t damageTimer = 0;
    const int32_t damageTime = 15;
    bool damaged = false;

    //Move
    int32_t moveTimer = 0;
    int32_t moveTime = 0;
    int32_t intervalTime = 0;

private:
    enum class State {
        Idle,
        Marker,
        Line,
        MarkerAndLine,
        Rush
    } state = State::Idle;

private:
    static Boss* sCurrent;
};

