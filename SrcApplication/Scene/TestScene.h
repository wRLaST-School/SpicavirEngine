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
};

