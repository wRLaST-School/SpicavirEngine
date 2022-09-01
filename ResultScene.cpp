#include "ResultScene.h"
#include "TextDrawer.h"
#include "wSoundManager.h"
#include "Input.h"

void ResultScene::Init()
{
	wTextureManager::Init();
	wSoundManager::Init();
	wTextureManager::LoadTexture("Resources/back.png", "back");
	back1 = Sprite("back");
	back2 = Sprite("back");

	back1.position = { 0, 360, 0 };
	back2.position = { 1280, 360, 0 };

	if (score.gradeMax > score.totDamage)
	{
		resultSpr = Sprite("Resources/result.png", "resultText");

		if (score.totDamage > score.gradeS)
		{
			rankSpr = Sprite("Resources/rankS.png", "rank");
		}
		else if (score.totDamage > score.gradeA)
		{
			rankSpr = Sprite("Resources/rankA.png", "rank");
		}
		else if (score.totDamage > score.gradeB)
		{
			rankSpr = Sprite("Resources/rankB.png", "rank");
		}
		else if (score.totDamage > score.gradeC)
		{
			rankSpr = Sprite("Resources/rankC.png", "rank");
		}
		else
		{
			rankSpr = Sprite("Resources/rankD.png", "rank");
		}
	}
	else
	{
		resultSpr = Sprite("Resources/clearResult.png", "resultText");

		sop.fontOptions.resolution = 128;
		sop.fontOptions.name = "MS UI Gothic";
		sop.size = 128;
	}

	wSoundManager::LoadWave("Resources/Sounds/resultBGM.wav", "resultbgm");
	bgm = wSoundManager::PlayBGM("resultbgm");
}

void ResultScene::Update()
{
	timerrr++;
	backPos -= 1;
	if (backPos <= -640) {
		backPos = 640;
	}

	back1.position.x = backPos;
	back2.position.x = backPos + 1280;

	if (timerrr > 120 && Input::Pad::DownAny())
	{
		endScene = true;
	}

	back1.UpdateMatrix();
	back2.UpdateMatrix();
}

void ResultScene::DrawBack()
{
	back1.Draw();
	back2.Draw();
}

void ResultScene::Draw3D()
{
}

void ResultScene::DrawSprite()
{
	if (score.gradeMax > score.totDamage)
	{
		resultSpr.position = Float3{ 640, 360, 0 };
		rankSpr.position = Float3{ 640, 360, 0 };
		resultSpr.UpdateMatrix();
		rankSpr.UpdateMatrix();

		resultSpr.Draw();
		rankSpr.Draw();
	}
	else
	{
		resultSpr.position = Float3{ 640, 360, 0 };
		resultSpr.UpdateMatrix();

		resultSpr.Draw();

		TextDrawer::DrawString(to_string(timer.timerSec), 640, 450, Align::Center, sop);
	}
}

void ResultScene::GiveTransitionData(GameSceneOutputs data)
{
	this->score.totDamage = data.finalScore.totDamage;
	this->timer = data.timer;
}
