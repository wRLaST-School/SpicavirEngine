#pragma once
#include "Matrix.h"
#include "wTextureManager.h"
#include "Sprite.h"
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

	void CreateTextureAndInit(
		float fov,
		int renderWidth,
		int renderHeight,
		int actualWidth,
		int actualHeight,
		TextureKey key
	);

	void SetViewportAndScissorsRect();
};

