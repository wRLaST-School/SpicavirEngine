#pragma once
#include "IScene.h"
#include <Camera.h>
#include <Camera2D.h>
class BTEditorScene :
    public IScene
{
public:
    ~BTEditorScene() {};
    void LoadResources() override;
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Camera cam;
    Camera2D cam2d;
};

