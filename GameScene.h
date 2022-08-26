#pragma once
#include "IScene.h"
#include "Matrix.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "TextDrawer.h"
#include "Timer.h"
#include "Boss.h"
#include "SceneManager.h"
using namespace Input;

class GameScene :
    public IScene
{
public:
    ~GameScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

    GameSceneOutputs GetTransitionData();

private:
    Camera camera;

    Player player;
    Model mPlayer;

    Sprite background;

    StringOptions stropt;

    Timer timer;

    Score score;
    Boss boss = Boss(&score);
};

