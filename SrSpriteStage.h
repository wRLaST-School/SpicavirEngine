#pragma once
#include "IRenderStage.h"
class SrSpriteStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();
};

