#pragma once
#include "IRenderStage.h"
class SrPostEffectStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();
};

