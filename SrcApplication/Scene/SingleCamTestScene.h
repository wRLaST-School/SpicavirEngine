#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
#include <Particle.h>
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

    Emitter<IParticle> e1;

    Emitter<IParticle> e2;

    Camera camera;
};

