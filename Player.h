#pragma once
#include "Object3D.h"
class Player :
    public Object3D
{
public:
    void Init();

    void Update();

    void Move();

    void Draw();

private:
    float spd = 0.08f;

    const float GRAV = 0.025f;
    const float JUMP_POWER = 0.5f;
    float vy = 0.f;

public:
    static void Load();

    static Player* Get();

    static void Set(Player* player);

private:
    static Player* sCurrent;
};

