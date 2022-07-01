#pragma once
#include "Matrix.h"
#include "wTextureManager.h"
#include "Sprite.h"
#include "wDirectX.h"

class FoveatedRenderingData
{
public:
	float fov;
	
	int renderWidth;
	int renderHeight;

	int actualWidth;
	int actualHeight;

	float aspectRatio;

	Matrix projection;

	TextureKey renderTex;

	Sprite sprite;

	wConstBuffer<ConstBufferDataVProj> vprojCB;

	void CreateTextureAndInit(
		float fov,
		int renderWidth,
		int renderHeight,
		int actualWidth,
		int actualHeight,
		TextureKey key
	);

	void SetViewportAndScissorsRect();
	void SetViewProjectionMatrix(Matrix viewMat);
};

struct NormalRendering {
	void SetVProjMatToDefault();
	void SetDefaultVProjMat(Matrix vproj);

	wConstBuffer<ConstBufferDataVProj> defaultVProj;
};