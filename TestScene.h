#pragma once
#include "IScene.h"
#include "wMath.h"
#include "Camera.h"
#include "Sprite.h"
class TestScene :
    public IScene
{
public:
    TestScene() { sceneId = "Test"; };
    ~TestScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Camera camera;
    Sprite cameraSpr;
    Camera xCam;
    Sprite xCamSpr;
    Camera yCam;
    Sprite yCamSpr;
    Camera zCam;
    Sprite zCamSpr;

    Camera finalScene;

    Object3D ray;
    Object3D sphere;

    Model mSphere;
    Model mCube;

    TextureKey whiteTex;

    Model sky;

    Object3D skysphere;
};

