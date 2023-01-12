#pragma once
#include <IScene.h>
#include <TextDrawer.h>
#include <Sprite.h>
class ResultScene
    : public IScene
{
    ~ResultScene() {  };
    void LoadResources() override;
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Sprite gameo;

    long long timer = 0;
};

