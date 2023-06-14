#pragma once
#include "Object3D.h"
class Boss :
    public Object3D
{
public:
    void Init();

    void Update();

    void Draw();

public:
    static void Load();

    static Boss* Get();

    static void Set(Boss* boss);

private:
    static Boss* sCurrent;
};

