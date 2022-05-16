#pragma once
#include "wConstBuffer.h"
#include "Model.h"
struct ConstBufferDataTransform {
	XMMATRIX mat;//3D•ÏŠ·s—ñ
};

class Object3D
{
public:
	Object3D() { transformCB.contents->mat = XMMatrixIdentity(); };
	void UpdateMatrix();
	void Draw(const XMMATRIX& viewProj);

	wConstBuffer<ConstBufferDataTransform> transformCB;

	XMFLOAT3 scale = { 1, 1, 1 };
	XMFLOAT3 rotation = { 0, 0, 0 };
	XMFLOAT3 posision = { 0, 0, 0 };

	XMMATRIX matWorld = XMMatrixIdentity();

	Object3D* parent = nullptr;

	Model* model = nullptr;
};

