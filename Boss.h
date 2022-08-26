#pragma once
#include "Object3D.h"
#include "Score.h"
class Boss :
    public Object3D
{
public:
    Boss(Score* score);

    void Update();

    void Draw();

    void Damage(int damage);

private:
    Score* pScore;
};

