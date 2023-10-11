#pragma once
#include "Object3D.h"
#include <Marker.h>
#include <LineAttack.h>
#include <OBBCollider.h>
#include <Score.h>
#include <GravSphere.h>
#include <BehaviorTree.h>
class Boss :
    public Object3D, public IComponent
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
    void InitBehaviorTree();

    void CastMarker(Float3 pos);

    BT::Status CastMarkerAim1Rand5();
    BT::Status CastMarkerLine3();

    void DrawMarkers();
    void UpdateMarkers();

    BT::Status CastLineTriple();
    void CastLine(Float3 pos, float angle);

    void UpdateLineAttacks();
    void DrawLineAttacks();

    BT::Status Rush();
    void RushEnd();

    BT::Status CastGravSphere();
    void GravSphereEnd();

    //state updates
    BT::Status RushUpdate();
    BT::Status LineAttackUpdate();
    BT::Status MarkerUpdate();
    BT::Status MarkerAndLineUpdate();
    BT::Status GravSphereUpdate();
    BT::Status Wait60Frame();

    bool IsPlayerInsideRushRange();

    void SelectMove();

    const OBBCollider& GetCollider();

    //デバッグ系
    void ShowImGui();

    //Cereal用
    template <class Archive>
    void serialize(Archive& ar) {
        ar(markerLine3Spacing_, lineAttackSpacing_, prepTime_, afterPrepWaitTime_,
            rushTime_, rushAfterTime_, rushDistance_,
            gravSphereSpd_, gravR_, gravitySpeed_, maxHomeRad_, gravSphereStayTime_);
    }

private:
    //BehaviorTree
    BT::BehaviorTree* tree_;

    //Marker
    static const int MAX_MARKERS = 256;
    eastl::array<Marker, MAX_MARKERS> markers_;

    float markerLine3Spacing_ = 20.f;

    //LineAttack
    eastl::list<LineAttack> lineAttacks_;

    float lineAttackSpacing_ = 15.f;

    //Rush
    int32_t prepTime_ = 30;
    int32_t afterPrepWaitTime_ = 30;
    int32_t rushTime_ = 60;
    int32_t rushAfterTime_ = 30;
    float rushDistance_ = 20.f;
    Float3 rushTarget_ = {};

    bool dealDamageOnHit_ = false;

    //Sphere
    std::unique_ptr<GravSphere> gravSphere_ = nullptr;
    int32_t gravSphereTime_ = 240;
    float gravSphereSpd_ = 0.2f;

    float gravR_ = 16.f;
    float gravitySpeed_ = 0.1f;
    float maxHomeRad_ = PIf / 90.f;

    int32_t gravSphereStayTime_ = 60;

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
        Rush,
        GravSphere
    } state_ = State::Idle;

private:
    static Boss* sCurrent;
};

