#pragma once
#include "IScene.h"
#include <Object3D.h>
#include <Camera.h>
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
    Object3D obj;

    Object3D sky;

    Camera camera;
};

