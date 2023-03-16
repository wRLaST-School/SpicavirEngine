#pragma once
#include "IToolMode.h"
#include <MoveBlock.h>
class MoveBlockTool :
    public IToolMode
{
public:
    void Update() override;
    void Draw() override;

    int width, height;
    MoveBlock::Direction dir;
};

