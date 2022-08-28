#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"

void GameScene::Init()
{
	wTextureManager::Init();
	wSoundManager::Init();
	camera.SetRenderSize(GetwWindow()->width, GetwWindow()->height);
	camera.nearZ = 0.1f;
	camera.farZ = 10000.0f;
	camera.fov = PI/2;

	camera.position.z = -1000;

	camera.projectionMode = ProjectionMode::Orthographic;
	camera.targetMode = CameraTargetMode::LookTo;

	mPlayer = Model("Player");
	mPlayerBullet = Model("PlayerBullet");
	mBoss = Model("Boss");
	mBossBit = Model("BossBits");

	boss.position.x += GetwWindow()->width / 4;
	boss.UpdateMatrix();

	Boss::SetCurrentBoss(&boss);
	
	player.model = &mPlayer;
	player.bulletModel = &mPlayerBullet;
	player.scale = { 25, 25, 25 };
	player.position = {0.0f, 0.0f, 0.0f};
	player.UpdateMatrix();

	boss.model = &mBoss;

	stropt.size = 24;
	stropt.fontOptions.name = "UDEV Gothic Regular";
	stropt.fontOptions.resolution = 192;

	wSoundManager::LoadWave("resources/sounds/shot.wav", "shot");
}

void GameScene::Update()
{
	camera.UpdateMatrix();
	player.Update();
	boss.Update();
	timer.Update();
}

void GameScene::DrawBack()
{

}

void GameScene::Draw3D()
{
	Camera::Set(camera);
	player.Draw();
	boss.Draw();
}

void GameScene::DrawSprite()
{
	//TextDrawer::DrawString((string)"x = " + to_string(player.position.x) + ", y = " + to_string(player.position.y), 100, 100, Align::TopLeft, stropt);
	timer.Draw();
}

GameSceneOutputs GameScene::GetTransitionData()
{
	return GameSceneOutputs{this->score};
}
