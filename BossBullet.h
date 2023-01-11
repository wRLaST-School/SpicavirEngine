#pragma once
#include "Object3D.h"
#include <Emitter.h>
#include <ParticleSample1.h>
class BossBullet :
    public Object3D
{
public:
    BossBullet(Vec3 vel, Emitter<ParticleSample1>* em) :vel(vel), em(em) { this->model = ModelManager::GetModel("BBullet"); em->active = true; };

    void Update();

    Vec3 vel;

    int totalTimer = 0;

    bool del = false;

    Emitter<ParticleSample1>* em;

    ~BossBullet() { em->active = false; }
};