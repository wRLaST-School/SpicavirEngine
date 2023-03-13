#pragma once
#include "IToolMode.h"
class SnakeHeadTool :
    public IToolMode
{
public:
    void Update() override;
    void Draw() override;

    int id;
};

