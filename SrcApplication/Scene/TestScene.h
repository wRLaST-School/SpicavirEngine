#pragma once
#include "IScene.h"
#include "Camera.h"
#include <Color.h>

class Circle : public IComponent
{
public:
    Circle(Vec2 pos, float r);

    bool Collide(const Circle& o);

    void Draw();

    Vec2 pos_;
    float r_;
    Color color_ = Color::White;
};

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

    Circle* circleA;
    Circle* circleB;
};

