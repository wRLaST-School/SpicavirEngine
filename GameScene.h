#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
class GameScene :
    public IScene
{
public:
    ~GameScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Object3D obj;
    Model mCube;

    Object3D sky;
    Model mSky;

    Camera camera;
};

