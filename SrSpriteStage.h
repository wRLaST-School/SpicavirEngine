#pragma once
#include "IRenderStage.h"
class SrSpriteStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();

    void Render();
    void DrawCommands(std::function<void(void)> cmd);

private:
    std::vector<std::function<void(void)>> commands_;
};

