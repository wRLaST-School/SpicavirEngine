#pragma once
#include "IScene.h"
class TitleScene :
    public IScene
{
public:
    ~TitleScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
};

