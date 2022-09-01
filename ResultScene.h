#pragma once
#include "IScene.h"
#include "SceneManager.h"
#include "Score.h"
#include "Timer.h"
#include "wSoundManager.h"
class ResultScene :
    public IScene
{
public:
    ResultScene() { sceneId = "Result"; };
    ~ResultScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

    void GiveTransitionData(GameSceneOutputs data);

private:
    Score score;
    Timer timer;

    Sprite back1;
    Sprite back2;

    int backPos = 0;

    Sprite resultSpr;
    Sprite rankSpr;

    IXAudio2SourceVoice* bgm;

    StringOptions sop;

    long long timerrr = 0;

    Sprite white;
    Float4 tranColor = { 0.f, 0.f, 0.f, 1.0f };
    int inTranTimer = 0;
    int outTranTimer = 0;

    bool inTran = true;

    bool outTran = false;
};

