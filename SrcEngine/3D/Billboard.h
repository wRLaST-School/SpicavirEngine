#pragma once
#include "Object3D.h"
#include "Camera.h"
class Billboard :
    public Object3D
{
public:
    Billboard()
    {
    };

    void UpdateMatrix();

    Camera* camera = nullptr;
};

class BillboardY :
    public Billboard
{
public:
    BillboardY()
    {
    }

    void UpdateMatrix();

    Vec3 upVec = { 0.f, 1.f, 0.f };
};
