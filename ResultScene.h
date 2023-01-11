#pragma once
#include <IScene.h>
#include <TextDrawer.h>
class ResultScene
    : public IScene
{
    ~ResultScene() {  };
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;
};

