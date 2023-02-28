#pragma once
#include "IScene.h"
class GameScene :
    public IScene
{
    ~GameScene() {};
    void LoadResources();
    void Init() override;
    void Update() override;
    void DrawBack()override {};
    void Draw3D() override {};
    void DrawSprite() override;
};

