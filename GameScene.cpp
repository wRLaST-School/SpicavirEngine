#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"

void GameScene::Init()
{
	wTextureManager::Init();
	camera.SetRenderSize(GetwWindow()->width, GetwWindow()->height);
	camera.nearZ = 0.1f;
	camera.farZ = 10000.0f;
	camera.fov = PI/2;

	camera.position.y = 1000;

	camera.rotation.x = PI / 2;

	camera.projectionMode = ProjectionMode::Orthographic;
	camera.targetMode = CameraTargetMode::LookTo;

	mPlayer = Model("cube");
	player.model = &mPlayer;
	player.bulletModel = &mPlayer;
	player.scale = { 25, 25, 25 };
	player.position = {0.0f, 0.0f, 0.0f};
	player.UpdateMatrix();

	stropt.size = 24;
	stropt.fontOptions.name = "UDEV Gothic Regular";
	stropt.fontOptions.resolution = 192;
}

void GameScene::Update()
{
	camera.UpdateMatrix();
	player.Update();
}

void GameScene::DrawBack()
{

}

void GameScene::Draw3D()
{
	Camera::Set(camera);
	player.Draw();
}

void GameScene::DrawSprite()
{
	TextDrawer::DrawString((string)"x = " + to_string(player.position.x) + ", z = " + to_string(player.position.z), 100, 100, Align::TopLeft, stropt);
}