#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
#include <Particle.h>
#include <ParticleSample1.h>
#include <ParticleSample2.h>
#include <Light.h>
#include <IPostEffector.h>
#include <PlaneCollider.h>
#include <SphereCollider.h>
#include <RayCollider.h>
class SingleCamTestScene :
    public IScene
{
public:
    SingleCamTestScene() {};
    ~SingleCamTestScene() {};
    void LoadResources() override;
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Object3D floor;
    PlaneCollider plCol;
    Object3D sphere;
    SphereCollider spCol;

    Ray ray;
    RayCollider rCol;

    Object3D sky;

    Camera camera;

    long long timer;
};

