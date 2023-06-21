#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include "SpTextureManager.h"
#include <Quaternion.h>
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
	void Draw(const TextureKey& key);

	void DrawCommands(const TextureKey& key);

	//加算合成
	void DrawAdd();
	//加算合成
	void DrawAdd(const TextureKey& key);

	//透過表示用
	void DrawAlpha();
	void DrawAlpha(const TextureKey& key);

	SpConstBuffer<ConstBufferDataTransform> transformCB;
	SpConstBuffer<Float4> brightnessCB;

	Float3 scale = { 1, 1, 1 };
	Quaternion rotation = Quaternion::Identity();
	Float3 position = { 0, 0, 0 };

	Matrix matWorld = Matrix::Identity();

	Object3D* parent = nullptr;

	Model* model = nullptr;

	float distanceToCam = 0;
	TextureKey alphaTexKey;
};

