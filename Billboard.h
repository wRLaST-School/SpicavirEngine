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

    Camera* camera;
};

