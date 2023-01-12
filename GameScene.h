#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <ParticleSample1.h>
#include <ParticleSample2.h>
#include <Emitter.h>
#include <Camera.h>
#include <Player.h>
#include <Boss.h>
#include <HP.h>
#include <BossHP.h>
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
    Player player;

    Boss boss;

    Object3D sky;
    Object3D floor;

    HP hp;
    BossHP bossHP;

    Emitter<ParticleSample1> e1;

    Emitter<ParticleSample2> e2;

    Camera camera;

    PointLight* light1;
};

