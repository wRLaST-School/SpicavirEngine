#pragma once
#include "IScene.h"
#include "Color.h"
#include <Background.h>
#include <Camera2D.h>
class GameScene :
    public IScene
{
public:
    ~GameScene() {};
    void LoadResources();
    void Init() override;
    void Update() override;
    void DrawBack()override {};
    void Draw3D() override {};
    void DrawSprite() override;

private:
    //karioki
    static const int maxbackp = 200;
    Vec2 backpos[maxbackp];
    Color backcolor[maxbackp];
    size_t timer = 0;

    
    int mapsize = 60;
    bool useSample = false;

    Background back;
    Camera2D cam;
};

