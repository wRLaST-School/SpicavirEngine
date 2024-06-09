#include "TestScene.h"
#include "RTVManager.h"
#include "SpSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"
#include <LineDrawer.h>

void TestScene::LoadResources()
{

}

void TestScene::Init()
{
	Camera* cam = this->AddComponent<Camera>("Camera");
	cam->UseDefaultParams();

	Camera::Set(*cam);
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
	
}
