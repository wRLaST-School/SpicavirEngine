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
#include "wSoundManager.h"
using namespace Input;

class GameScene :
    public IScene
{
public:
    GameScene() { sceneId = "Game"; };
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

    Model mBoss;
    Model mPlayerBullet;
    Model mBossBit;

    StringOptions stropt;

    Timer timer;

    Sprite back1;
    Sprite back2;

    Sprite ope;

    IXAudio2SourceVoice* bgm;

    int backPos = 0;

    Score score;
    Boss boss = Boss(&score);
};

