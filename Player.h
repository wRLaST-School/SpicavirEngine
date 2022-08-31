#pragma once
#include "Object3D.h"
#include "PlayerBullet.h"
#include "Sprite.h"
class Player :
    public Object3D
{
public:
    Player() {};

    void Update();

    void Draw();

    static Player* GetCurrentPlayerPtr();

    static void SetCurrentPlayer(Player* ptr);

    void Damage();
    
    void HealthSprInit();
    void DrawHP();

    float health = 3.0f;

    list<PlayerBullet> bullets;

    Model* bulletModel;

    float r = 8;

private:

    Sprite healthSpr[3];
    Sprite healthBackSpr;

    enum class State
    {
        Normal,
        Slow,
        Dodge,
    };

    State state = State::Normal;

    int attackCD = 0;
    const int attackCDDef = 2;

    const float speed = 8;
    const float slowspeed = speed / 2;

    int dodgeCD = 0;
    const int dodgeCDDef = 60;

    int immuneTime = 0;
    const int immuneTimeDef = 60;

    long long dodgeTimer = 0;
    const int dodgeImmuneTime = 24;
    bool countered = false;

    Vec3 moveVec;
private:
    void NormalUpdate();
    void NormalMove();

    void SlowUpdate();
    void SlowMove();

    void UpdateBullets();
    void NormalAttack();
    void SlowAttack();

    void DodgeUpdate();
    void CounterAttack();

    static Player* pPlayer;
};

