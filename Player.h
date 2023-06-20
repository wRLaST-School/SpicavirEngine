#pragma once
#include "Object3D.h"
class Player :
    public Object3D
{
public:
    void Init();

    void Update();

    void Draw();

public:
    static void Load();

    static Player* Get();

    static void Set(Player* player);

private:
    static Player* sCurrent;
};

