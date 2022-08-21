#pragma once
#include "Object3D.h"
#include "PlayerBullet.h"
class Player :
    public Object3D
{
public:
    Player() {};

    void Update();

    void Draw();

    float health = 3.0f;

    list<PlayerBullet> bullets;

    Model* bulletModel;

private:
    enum class State
    {
        Normal,
        Slow,
        Dodge,
    };

    State state = State::Normal;

    int dodgeCD = 0;
    const int dodgeCDDef = 60;

    int attackCD = 0;
    const int attackCDDef = 12;

    const float speed = 16;
    const float slowspeed = speed / 2;

    int dodgeTimer = 0;
    const int dodgeImmuneTime = 12;
private:
    void NormalUpdate();
    void NormalMove();

    void SlowUpdate();
    void SlowMove();

    void UpdateBullets();
    void AttackCommon();

    void DodgeUpdate();
};

