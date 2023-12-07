#pragma once
#include "IScene.h"
#include <Camera.h>
#include <Circle.h>
class ServerScene :
    public IScene
{
public:
    ~ServerScene() {};
    void LoadResources();
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

    DWORD Threadfunc();

private:
    Camera cam;

    CircleCollider* circleA;
    CircleCollider* circleB;
};

