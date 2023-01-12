#pragma once
#include "Object3D.h"
#include <ParticleSample2.h>
#include <Emitter.h>
#include <ParticleSample1.h>
class Player :
    public Object3D
{
public:
    Player() { position.z = -5; };
    void Update();
    void Draw();

    void Dodge();

    float speed = 0.1f;
    float dodgespd = 0.4f;

    static Player* Get();
    static void Set(Player* p);

    static Player* current;

    int health = 10;

    void Damage();

    Vec3 dodgeVel;
    int dodgeTimer = 0;
    int dodgeTime = 13;
    bool usingFlash = false;
    bool immune = false;
    int flashCD = 5;

    long long int successTimer = 11;
    long long int successTime = 10;

    Emitter<ParticleSample1> em;
};