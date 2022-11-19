#pragma once
#include "IScene.h"
#include "SpMath.h"
#include "Camera.h"
#include "Sprite.h"
#include "Billboard.h"
#include <Particle.h>
class TestScene :
    public IScene
{
public:
    ~TestScene() {};
    void LoadResources();
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
    BillboardY pane;
    Billboard pane2;

    TextureKey whiteTex;

    Model sky;

    Emitter<IParticle> emitter;

    Object3D skysphere;
};

