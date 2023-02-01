#pragma once
#include "IRenderStage.h"
class SrOpaqueStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();
};

