#pragma once
#include "Object3D.h"
class Player :
    public Object3D
{
public:
    void Update();
    void Draw();

    float speed = 0.25f;
    float dodgespd = 1.8f;
    
    int immuneTimer;
    int immuneTime;

    static Player* Get();
    static void Set(Player* p);

    static Player* current;

    int health = 10;

    void Damage();
};

