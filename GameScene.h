#pragma once
#include "IScene.h"
#include "Matrix.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
const int TileQuant = 10;
using namespace Input;

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
	Sprite osrspr;

    Camera camera;
	Camera topCamera;

    Float4 color = {255.0f, 255.0f, 255.0f, 255.0f};
	Float3 hsv = {0.0f, 100.0f, 100.0f};

    Matrix vMat;

	//去年授業の自作関数コンテストから借りました
	Float3 ConvertHSVtoRGB(Float3 hsv)
	{
		float h = hsv.x;
		float s = hsv.y;
		float v = hsv.z;

		while (h < 0) { h += 360; }
		while (h > 360) { h -= 360; }
		if (s < 0) { s = 0; }
		if (s > 100) { s = 100; }
		if (v < 0) { v = 0; }
		if (v > 100) { v = 100; }

		s = 255 * s / 100.0f;
		v = 255 * v / 100.0f;

		float MAX = v;
		float MIN = MAX - ((s / 255.0f) * MAX);

		if (h >= 0 && h <= 60) {
			return { MAX, ((h / 60.0f) * (MAX - MIN) + MIN), MIN };
		}
		else if (h >= 61 && h <= 120) {
			return { (((120 - h) / 60.0f) * (MAX - MIN) + MIN), MAX, MIN };
		}
		else if (h >= 121 && h <= 180) {
			return { MIN, MAX, (((h - 120) / 60.0f) * (MAX - MIN) + MIN) };
		}
		else if (h >= 181 && h <= 240) {
			return { MIN, (((240 - h) / 60.0f) * (MAX - MIN) + MIN), MAX };
		}
		else if (h >= 241 && h <= 300) {
			return { (((h - 240) / 60.0f) * (MAX - MIN) + MIN), MIN, MAX };
		}
		else if (h >= 301 && h <= 360) {
			return { MAX, MIN, (((360 - h) / 60.0f) * (MAX - MIN) + MIN) };
		}

		return { 0, 0, 0 };
	}
};

