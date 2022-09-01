#include "GameScene.h"
#include "Input.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"
#include "wSoundManager.h"

void GameScene::Init()
{
	wTextureManager::Init();
	wSoundManager::Init();
	FontManager::InitData();

	wSoundManager::LoadWave("Resources/Sounds/gameBGM.wav", "gameBGM");
	bgm = wSoundManager::PlayBGM("gameBGM");

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
	player.scale = { 15, 15, 15 };
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

	wSoundManager::LoadWave("Resources/Sounds/shot.wav", "shot");
	wSoundManager::LoadWave("Resources/Sounds/marker.wav", "marker");
	wSoundManager::LoadWave("Resources/Sounds/counterSuccess.wav", "csuccess");
	wSoundManager::LoadWave("Resources/Sounds/enShot.wav", "eshot");
	wSoundManager::LoadWave("Resources/Sounds/dodge.wav", "dodge");
	wSoundManager::LoadWave("Resources/Sounds/takeDamage.wav", "damage");

	wTextureManager::LoadTexture("Resources/back.png", "back");

	back1 = Sprite("back");
	back2 = Sprite("back");

	back1.position = { 0, 360, 0 };
	back2.position = { 1280, 360, 0};

	wTextureManager::LoadTexture("Resources/health.png", "health");
	wTextureManager::LoadTexture("Resources/healthBack.png", "healthBack");
	wTextureManager::LoadTexture("Resources/warn.png", "warn");

	player.HealthSprInit();

	player.pCamera = &camera;

	ope = Sprite("Resources/operation.png", "operation");
	white = Sprite("Resources/white.png", "whitetr");
}

void GameScene::Update()
{
	if (inTran)
	{
		inTranTimer++;

		if (inTranTimer > 64)
		{
			inTran = false;
		}

		tranColor = { 0.f, 0.f, 0.f, 1.0f - (float)inTranTimer / 64 };
	}

	if (outTran)
	{
		outTranTimer++;

		if (outTranTimer > 64)
		{
			endScene = true;
		}

		tranColor = { 0.f, 0.f, 0.f, (float)outTranTimer / 64 };
	}

	backPos-= 1;
	if (backPos <= -640) {
		backPos = 640;
	}

	back1.position.x = backPos;
	back2.position.x = backPos + 1280;

	back1.UpdateMatrix();
	back2.UpdateMatrix();

	if (!outTran)
	{
		camera.UpdateShake();
		camera.UpdateMatrix();
		player.Update();
		boss.Update();
		timer.Update();
		score.Update();

		if (score.totDamage >= score.gradeMax)
		{
			outTran = true;
		}

		if (timer.timerSec < 0)
		{
			outTran = true;
		}

		if (player.health <= 0)
		{
			outTran = true;
		}
	}

	ope.position = {640, 360, 0};
	ope.UpdateMatrix();
}

void GameScene::DrawBack()
{
	back1.Draw();
	back2.Draw();
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
	ope.Draw();
	score.Draw();
	//TextDrawer::DrawString((string)"x = " + to_string(player.position.x) + ", y = " + to_string(player.position.y), 100, 100, Align::TopLeft, stropt);
	timer.Draw();
	player.DrawHP();

	//TextDrawer::DrawString(to_string(player.health), 100, 100, Align::TopLeft);

	if (inTran || outTran)
	{
		white.position = { 640, 360, 0 };
		white.scale = { 2000, 2000, 1 };
		white.brightness = tranColor;
		white.UpdateMatrix();
		white.Draw();
	}
}

GameSceneOutputs GameScene::GetTransitionData()
{
	return GameSceneOutputs{this->score, this->timer};
}
