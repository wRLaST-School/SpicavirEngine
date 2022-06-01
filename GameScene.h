#pragma once
#include "IScene.h"
#include "Matrix.h"
#include "Model.h"
#include "Object3D.h"
class GameScene :
    public IScene
{
public:
    ~GameScene() {};
    void Init() override;
    void Update() override;
    void Draw() override;

private:
    Matrix wMat;

    Model model = Model("Resources/Models/square.obj");

    list<Object3D> objects;

    Matrix vproj;

    TextureKey texture;
    TextureKey tex2;
};

