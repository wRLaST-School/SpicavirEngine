#pragma once
#include "IScene.h"
#include "Matrix.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "FoveatedRenderingParamater.h"
const int TileQuant = 10;

enum class ControllMode {
    BIOHAZERD,
    TPS
};

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

private:

    Model model;
    Model cubem;
    Model sky;

    Object3D monkey;
    Object3D floor[TileQuant][TileQuant];

    Object3D skysphere;

    Matrix vproj;

    TextureKey texture;

    ControllMode ctrlmode = ControllMode::BIOHAZERD;

    Sprite spr;

    Object3D camera;

    FoveatedRenderingData center;
    FoveatedRenderingData middle;
    FoveatedRenderingData outer;

    NormalRendering nrdata;

    enum class RenderingMode {
        normal,
        foveated,
        vr
    } renderMode= RenderingMode::normal;

    Matrix vMat;

    struct EyeResource {
        FoveatedRenderingData center;
        FoveatedRenderingData middle;
        FoveatedRenderingData outer;
    };

    EyeResource eyeRes[2];
};

