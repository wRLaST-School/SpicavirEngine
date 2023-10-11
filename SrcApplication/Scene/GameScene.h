#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
#include <Boss.h>
#include <Player.h>
#include <CameraController.h>
class GameScene :
    public IScene
{
public:
    ~GameScene() {};
    void LoadResources() override;
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Boss* boss_;
    Player* player_;

    CameraController cam_;
};

