#pragma once
#include "Object3D.h"
#include "Score.h"
#include "BossBullet.h"
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

    Model* bulletModel;

private:
    Score* pScore;
    State state = State::Idle;
    State secondaryState = State::Idle;

    void IdleUpdate();
    void SpreadUpdate();
    void AimUpdate();
    void CircleUpdate();
    void MarkerUpdate();

    void UpdateBullets();

    void NWay(Float3 pos, int ways, float speed, float centerRad, float radDiff);

    static Boss* pCurrentBoss;

    int attackTimer = 0;
    const int attackTime1 = 60;
    const int attackTime2 = 120;

    int attackingTimer = 0;
    const int spreadTime = 60;
    const int aimTime = 120;
    const int circleTime = 120;
    const int markerTime = 240;

    const int attackingTimes[4] =
    {
        60,//spread
        120,//aim
        120,//circle
        240//marker
    };


    int tAttacks = 0;

    bool attackEnd = false;

    float spreadShotCenter;

    list<BossBullet> bullets;

    Float3 nextPos = { 0,0,0 };
    Float3 lastPos = { 0,0,0 };
};

