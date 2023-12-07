#pragma once
#include "IScene.h"
#include "Camera.h"
#include <Color.h>
#include <Circle.h>

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
    Camera cam;

    CircleCollider* circleA;
    CircleCollider* circleB;
};

