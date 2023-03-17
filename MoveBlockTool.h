#pragma once
#include "IToolMode.h"
#include <MoveBlock.h>
class MoveBlockTool :
    public IToolMode
{
public:
    void Update() override;
    void Draw() override;

    int width = 60, height = 60;
    MoveBlock::Direction dir;
};

