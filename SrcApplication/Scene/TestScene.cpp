#include "TestScene.h"
#include "RTVManager.h"
#include "SpSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"
#include <LineDrawer.h>
#include <SpDS.h>

void TestScene::LoadResources()
{
	SpTextureManager::LoadTexture("Assets/Images/white.png", "white");
	SpTextureManager::LoadTexture("Assets/Images/hexagonPattern.jpg", "hex_jpg");
	SpTextureManager::LoadTexture("Assets/Images/hexagonPattern.dds", "hex_dds");
}

void TestScene::Init()
{
	Camera::Set(cam);
}

void TestScene::Update()
{
}

void TestScene::DrawBack()
{

}

void TestScene::Draw3D()
{
}

void TestScene::DrawSprite()
{
	SpDS::DrawRotaGraph(0, 0, 1.f, 1.f, 0.f, "hex_dds", Anchor::TopLeft);

	SpDS::DrawRotaGraph(1980, 0, 1.f, 1.f, 0.f, "hex_jpg", Anchor::TopRight);
}
