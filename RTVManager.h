#pragma once
#include "wTextureManager.h"
#include "RenderTarget.h"
class RTVManager
{
public:
	static void SetRenderTargetToBackBuffer(UINT bbIndex);
	static void SetRenderTargetToTexture(TextureKey key);

	static RenderTarget* GetCurrentRenderTarget();

	static RTVManager& GetInstance();

private:
	map<TextureKey, RenderTarget> rtMap;
	RenderTarget bbrt[2];
	RenderTarget* currentRT;
};

