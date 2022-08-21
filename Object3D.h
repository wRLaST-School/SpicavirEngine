#pragma once
#include "wConstBuffer.h"
#include "Model.h"
#include "wTextureManager.h"
struct ConstBufferDataTransform {
	Matrix mat;//3D変換行列
};

class Object3D
{
public:
	Object3D() { transformCB.contents->mat = Matrix::Identity(); *brightnessCB.contents = { 1.0f, 1.0f, 1.0f, 1.0f }; };
	void UpdateMatrix();
	void Draw();

	//別途読み込んだテクスチャを使う場合
	void Draw(TextureKey key);

	wConstBuffer<ConstBufferDataTransform> transformCB;
	wConstBuffer<Float4> brightnessCB;

	Float3 scale = { 1.f, 1.f, 1.f };
	Float3 rotation = { 0.f, 0.f, 0.f };
	Float3 position = { 0.f, 0.f, 0.f };

	Matrix matWorld = Matrix::Identity();

	Object3D* parent = nullptr;

	Model* model = nullptr;
};

