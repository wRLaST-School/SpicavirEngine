#pragma once
#include "IScene.h"
#include <Camera2D.h>

class EditorScene :
    public IScene
{
public:
    ~EditorScene() {};
    void LoadResources();
    void Init() override;
    void Update() override;
    void DrawBack()override {};
    void Draw3D() override {};
    void DrawSprite() override;

private:
    Camera2D cam;
};

