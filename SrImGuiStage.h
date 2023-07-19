#pragma once
#include "IRenderStage.h"
class SrImGuiStage :
    public IRenderStage
{
public:
    void Init() override;
    void PreDraw() override;
    void Render() override;
    void PostDraw() override;

    void DrawCommands(std::function<void(void)> cmd) override;
private:
};

