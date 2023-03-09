#pragma once
#include "IToolMode.h"
class MapChipTool :
    public IToolMode
{
public:
    MapChipTool(int chipID) { chipNum = chipID; };
    void Update() override;
    static int chipNum;
};

