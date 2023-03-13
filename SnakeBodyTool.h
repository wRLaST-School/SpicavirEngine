#pragma once
#include "IToolMode.h"
class SnakeBodyTool :
    public IToolMode
{
public:
    void Update() override;
    void Draw() override;

    int id;
    int order;
};

