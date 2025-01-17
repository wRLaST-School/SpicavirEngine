#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include "SpTextureManager.h"
#include <Quaternion.h>
#include <ComponentFactory.h>

struct ConstBufferDataTransform {
	Matrix mat;//3D変換行列
};

struct ConstBufferDataMisc {
	Float4 rimColor;
	float rimStrength;
	Float3 padding;
	float dissolveStrength;
	Float3 padding2;
};

class Object3D : public IComponent
{
public:
	ComponentFactoryRegister(Object3D)

	Object3D() { transformCB.contents->mat = Matrix::Identity(); *brightnessCB.contents = { 1.0f, 1.0f, 1.0f, 1.0f }; miscCB.contents->rimColor = { 1.f, 0.f, 0.f, 1.f }; };
	void UpdateMatrix();
	void DecomposeMatrix();

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

	//Inspector Window用
	void DrawParams();
	void DrawGizmo();

	void ReadParamJson([[maybe_unused]]const nlohmann::json& jsonObject) override;
	void WriteParamJson([[maybe_unused]] nlohmann::json& jsonObject) override;

	SpConstBuffer<ConstBufferDataTransform> transformCB;
	SpConstBuffer<Float4> brightnessCB;
	SpConstBuffer<ConstBufferDataMisc> miscCB;

	Float3 scale = { 1, 1, 1 };
	Quaternion rotation = Quaternion::Identity();
	Float3 rotationE = {};
	enum class RotMode{
		Euler,
		Quaternion
	} rotMode = RotMode::Quaternion;

	enum class BlendMode {
		Opaque,
		Add,
		Alpha
	} blendMode = BlendMode::Opaque;

	Float3 position = { 0, 0, 0 };

	Matrix matWorld = Matrix::Identity();

	Object3D* parent = nullptr;

	TextureKey texture = "";

	Model* model = nullptr;

	float distanceToCam = 0;
	TextureKey alphaTexKey;
};

