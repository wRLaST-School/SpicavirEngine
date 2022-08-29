#pragma once
#include "Object3D.h"
#include "Score.h"
class Boss :
    public Object3D
{
public:
    enum class State {
        Idle,
        Spread,
        Aim,
        Circle,
        Marker
    };

    Boss(Score* score);

    void Update();

    void Draw();

    void Damage(int damage);

    static Boss* GetCurrentBossPtr();

    static void SetCurrentBoss(Boss* ptr);

private:
    Score* pScore;
    State state = State::Idle;
    State secondaryState = State::Idle;

    void IdleUpdate();
    void SpreadUpdate();
    void AimUpdate();
    void CircleUpdate();
    void MarkerUpdate();

    static Boss* pCurrentBoss;

    int attackTimer = 0;
    const int attackTime1 = 60;
    const int attackTime2 = 120;

    int tAttacks = 0;

    bool attackEnd = false;
};

