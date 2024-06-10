#pragma once
#include "IScene.h"
class SceneFromFile :
    public IScene
{
public:
    SceneFromFile(std::string filePath);
    ~SceneFromFile() {};
    void LoadResources();
    void Init();
    void Update();
    void DrawBack();
    void Draw3D();
    void DrawSprite();

private:
    std::string filePath_;
};

