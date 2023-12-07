#pragma once
#include <Circle.h>
#include <Camera.h>
#include <IScene.h>
class ClientScene : public IScene
{
public:
    ~ClientScene() {};
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

