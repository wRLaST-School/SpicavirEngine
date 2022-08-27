#pragma once
#include "Object3D.h"
#include "Score.h"
class Boss :
    public Object3D
{
public:
    enum class State {
        Idle
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
    void IdleUpdate();

    static Boss* pCurrentBoss;
};

