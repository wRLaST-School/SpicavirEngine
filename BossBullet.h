#pragma once
#include "Object3D.h"
class BossBullet :
    public Object3D
{
public:
    BossBullet(Vec3 vel) :vel(vel) { this->model = ModelManager::GetModel("BBullet"); };

    void Update();

    Vec3 vel;

    int totalTimer = 0;

    bool del = false;
};