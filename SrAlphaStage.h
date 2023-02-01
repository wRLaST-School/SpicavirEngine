#pragma once
#include "IRenderStage.h"
class SrAlphaStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();
};

