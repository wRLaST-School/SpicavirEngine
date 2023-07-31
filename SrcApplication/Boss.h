#pragma once
#include "Object3D.h"
#include <Marker.h>
#include <LineAttack.h>
#include <OBBCollider.h>
#include <Score.h>
class Boss :
    public Object3D
{
public:
    void Init();

    void Update();

    void Draw();
    
    void Damage(int32_t damage = 0);

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
    void RushEnd();

    //state updates
    void RushUpdate();
    void LineAttackUpdate();
    void MarkerUpdate();
    void MarkerAndLineUpdate();
    void IdleUpdate();

    void SelectMove();

    const OBBCollider& GetCollider();

private:
    //Marker
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers_;

    //LineAttack
    eastl::list<LineAttack> lineAttacks_;

    float markerLine3Spacing_ = 20.f;
    float lineAttackSpacing_ = 15.f;

    //Rush
    int32_t prepTime_ = 30;
    int32_t afterPrepWaitTime_ = 30;
    int32_t rushTime_ = 60;
    int32_t rushAfterTime_ = 30;
    float rushDistance_ = 20.f;
    Float3 rushTarget_ = {};

    bool dealDamageOnHit_ = false;

    //Idle
    int32_t timesAttacked_ = 0;

    //Collision
    OBBCollider col_;

    //Damage
    int32_t damageTimer_ = 0;
    const int32_t damageTime = 15;
    bool damaged_ = false;

    //Move
    int32_t moveTimer_ = 0;
    int32_t moveTime_ = 0;
    int32_t intervalTime_ = 0;

private:
    enum class State {
        Idle,
        Marker,
        Line,
        MarkerAndLine,
        Rush
    } state_ = State::Idle;

private:
    static Boss* sCurrent;
};
