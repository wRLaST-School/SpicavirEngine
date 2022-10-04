#pragma once
#include "Object3D.h"
#include "Camera.h"
class Billboard :
    public Object3D
{
public:
    Billboard()
    {
        Object3D::Object3D();
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
        Billboard::Billboard();
    }

    void UpdateMatrix();

    Vec3 upVec = { 0.f, 1.f, 0.f };
};
