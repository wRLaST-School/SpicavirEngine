#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"

void GameScene::Init()
{
	wTextureManager::Init();
	wSoundManager::Init();

	wTextureManager::LoadTexture("Resources/white.png", "white");
	score.Init();

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

	Player::SetCurrentPlayer(&player);

	boss.model = &mBoss;
	boss.bulletModel = &mPlayerBullet;

	Marker::SetMarkerTextures(
		wTextureManager::LoadTexture("Resources/marker.png", "outerMarker"),
		wTextureManager::LoadTexture("Resources/marker.png", "innerMarker")
	);

	stropt.size = 24;
	stropt.fontOptions.name = "UDEV Gothic Regular";
	stropt.fontOptions.resolution = 192;

	wSoundManager::LoadWave("Resources/sounds/shot.wav", "shot");
}

void GameScene::Update()
{
	camera.UpdateMatrix();
	player.Update();
	boss.Update();
	timer.Update();
	score.Update();

	if (score.totDamage >= score.gradeMax)
	{
		endScene = true;
	}

	if (timer.timerSec < 0)
	{
		endScene = true;
	}

	if (player.health <= 0)
	{
		endScene = true;
	}
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
	boss.DrawMarkers();
	score.Draw();
	//TextDrawer::DrawString((string)"x = " + to_string(player.position.x) + ", y = " + to_string(player.position.y), 100, 100, Align::TopLeft, stropt);
	timer.Draw();
}

GameSceneOutputs GameScene::GetTransitionData()
{
	return GameSceneOutputs{this->score};
}
