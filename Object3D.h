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
	Object3D() { transformCB.contents->mat = Matrix::Identity(); };
	void UpdateMatrix();
	void Draw(const Matrix& viewProj);

	//別途読み込んだテクスチャを使う場合
	void Draw(const Matrix& viewProj, TextureKey key);

	wConstBuffer<ConstBufferDataTransform> transformCB;

	Float3 scale = { 1, 1, 1 };
	Float3 rotation = { 0, 0, 0 };
	Float3 posision = { 0, 0, 0 };

	Matrix matWorld = Matrix::Identity();

	Object3D* parent = nullptr;

	Model* model = nullptr;
};

