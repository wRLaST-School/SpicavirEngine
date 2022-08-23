#pragma once
#include "Object3D.h"
class Boss :
    public Object3D
{
public:
    Boss();

    void Update();

    void Draw();

    void Damage(int damage);

private:
    int damage = 0;
};

