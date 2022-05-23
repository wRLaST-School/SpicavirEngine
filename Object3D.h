#pragma once
#include "wConstBuffer.h"
#include "Model.h"
struct ConstBufferDataTransform {
	Matrix mat;//3D•ÏŠ·s—ñ
};

class Object3D
{
public:
	Object3D() { transformCB.contents->mat = Matrix::Identity(); };
	void UpdateMatrix();
	void Draw(const Matrix& viewProj);

	wConstBuffer<ConstBufferDataTransform> transformCB;

	Float3 scale = { 1, 1, 1 };
	Float3 rotation = { 0, 0, 0 };
	Float3 posision = { 0, 0, 0 };

	Matrix matWorld = Matrix::Identity();

	Object3D* parent = nullptr;

	Model* model = nullptr;
};

