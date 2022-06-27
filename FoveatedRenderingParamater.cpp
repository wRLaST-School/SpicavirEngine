#include "FoveatedRenderingParamater.h"
#include "RTVManager.h"
#include "wDirectX.h"

void FoveatedRenderingData::CreateTextureAndInit(float fov, int renderWidth, int renderHeight, int actualWidth, int actualHeight, TextureKey key)
{
	this->fov = fov;
	this->renderWidth = renderWidth;
	this->renderHeight = renderHeight;
	this->actualWidth = actualWidth;
	this->actualHeight = actualHeight;

	aspectRatio = (float)renderWidth / (float)renderHeight;

	projection = Matrix::Projection(fov, aspectRatio, 0, 1000);

	RTVManager::CreateRenderTargetTexture(renderWidth, renderHeight, key);
	
	sprite = Sprite(key);

	sprite.position = Float3{(float)GetwWindow()->width / 2, (float)GetwWindow()->height / 2, 0};
	sprite.scale = Float3{(float)actualWidth / renderWidth, (float)actualHeight / renderHeight, 1};
	sprite.UpdateMatrix();

	renderTex = key;
}

void FoveatedRenderingData::SetViewportAndScissorsRect()
{
	D3D12_VIEWPORT viewport{};

	viewport.Width = renderWidth;
	viewport.Height = renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + renderWidth;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + renderHeight;       // 切り抜き座標下

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);
}
