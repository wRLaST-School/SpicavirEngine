#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "wSoundManager.h"
class TitleScene :
    public IScene
{
public:
    TitleScene() { sceneId = "Title"; };
    ~TitleScene() { /*delete bgm; */};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    Sprite titleSpr;
    int timer = 0;
    Sprite tutorialSpr;

    Sprite back1;
    Sprite back2;

    int backPos = 0;

    int xPos = 0;

    int tutorialTimer = 0;
    bool tutorialMoving = false;

    Sprite tut1;
    Sprite tut2;
    Sprite tut3;

    IXAudio2SourceVoice* bgm;

    Sprite white;
    Float4 tranColor = {0.f, 0.f, 0.f, 1.0f};
    int inTranTimer = 0;
    int outTranTimer = 0;

    bool inTran = true;

    bool outTran = false;
};

