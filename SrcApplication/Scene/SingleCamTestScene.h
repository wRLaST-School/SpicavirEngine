#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
#include <Particle.h>
#include <ParticleSample1.h>
#include <ParticleSample2.h>
#include <Light.h>
class SingleCamTestScene :
    public IScene
{
public:
    ~SingleCamTestScene() {};
    void LoadResources() override;
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Object3D pane;

    Object3D sky;

    Emitter<ParticleSample1> e1;

    Emitter<ParticleSample2> e2;

    Camera camera;

    PointLight* light1;
};

