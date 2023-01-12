#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "SoundManager.h"
class TitleScene :
    public IScene
{
public:
    ~TitleScene() { /*delete bgm; */ };
    void LoadResources();
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    long long timer = 0;

    Sprite title;
};
