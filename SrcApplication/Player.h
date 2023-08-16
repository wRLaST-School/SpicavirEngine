#pragma once
#include "Object3D.h"
#include <Emitter.h>
#include <CounterParticle.h>
#include <OBBCollider.h>
class Player :
    public Object3D
{
public:
    void Init();

    void Update();

    void Move();
    void GravMove();
    void ClampPos();
    void MoveTo(Float3 newPos);
    void DamageUpdate();
    void Damage();
    void Draw();

    void DodgeUpdate();
    void IdleMoveUpdate();
    void Dodge();

    void SlashUpdate1();
    void SlashUpdate2();
    void SlashUpdate3();

    void Slash1();
    void Slash2();
    void Slash3();

    const OBBCollider& GetCollider();

    float r2d = .5f;

    enum class State {
        Idle,
        Move,
        Dodge,
        Slash1,
        Slash2,
        Slash3
    } GetState();

    struct DodgeData {
        int32_t dodgeTimer;
        int32_t iFrame;
        Float3 startCameraPos;
        Float3 dodgeEndPlayerPos;
    }
    GetDodgeData();

private:
    float spd_ = 0.125f;

    const float GRAV = 0.025f;
    const float JUMP_POWER = 0.5f;
    float vy_ = 0.f;

    Float3 rotTemp_ = { 0, 0, 0 };

    int32_t damageTimer_ = 0;

    State state_ = State::Idle;

private:
    Vec3 dodgeVec_ = Vec3();
    int32_t dodgeTimer_ = 0;
    int32_t iFrame = 25;

    float dodgeSpd_ = 0.48f;

    int32_t dodgeSucceededTimer_ = 0;

    Float3 dodgeStartCameraPos_;
    Float3 dodgeEndPlayerPos_;

    int32_t slashTimer_ = 0;
    const int32_t slashTime = 40;
    const int32_t slash3Time = 60;

    int32_t slashDamage_ = 150;
    int32_t slash3Damage_ = 300;

    const float slashDist = 3.f;
    
    Float3 slashScale_ = { 5.f, 1.f, 3.f };

    OBBCollider slashCol_;

    bool slashRegistered_ = false;
    bool slashHit_ = false;

    Emitter<CounterParticle> counterEmitter_;

    OBBCollider col_;

public:
    static void Load();

    static Player* Get();

    static void Set(Player* player);

private:
    static Player* sCurrent;
};

