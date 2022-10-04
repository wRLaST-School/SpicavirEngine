#pragma once
#include "IScene.h"
#include "Matrix.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
using namespace Input;

enum class ControllMode {
    BIOHAZERD,
    TPS
};

class MonkeyScene :
    public IScene
{
public:
    ~MonkeyScene() {};
    void Init() override;
    void Update() override;
    void DrawBack() override;
    void Draw3D() override;
    void DrawSprite() override;

private:
    const static int TileQuant = 10;

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
    Sprite osrspr;

    Camera camera;
    Camera topCamera;

    Float4 color = { 255.0f, 255.0f, 255.0f, 255.0f };
    Float3 hsv = { 0.0f, 100.0f, 100.0f };

    Matrix vMat;

    PointLight* pointLight;
};