#pragma once
#include "IScene.h"
#include "SceneManager.h"
#include "Score.h"
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
};

