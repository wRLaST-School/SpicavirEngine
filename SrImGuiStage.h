#pragma once
#include "IRenderStage.h"
class SrImGuiStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();
};

