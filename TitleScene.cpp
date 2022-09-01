#include "TitleScene.h"
#include "wSoundManager.h"
#include "Input.h"

void TitleScene::Init()
{
	wTextureManager::Init();
	wSoundManager::Init();

	titleSpr = Sprite("Resources/title.png", "title");

	wTextureManager::LoadTexture("Resources/back.png", "back");

	tut1 = Sprite("Resources/tutorial1.png", "tut1");
	tut2 = Sprite("Resources/tutorial2.png", "tut2");
	tut3 = Sprite("Resources/tutorial3.png", "tut3");

	back1 = Sprite("back");
	back2 = Sprite("back");

	back1.position = { 0, 360, 0 };
	back2.position = { 1280, 360, 0 };

	wSoundManager::LoadWave("Resources/Sounds/titleBGM.wav", "titleBGM");
	bgm = wSoundManager::PlayBGM("titleBGM");

	white = Sprite("Resources/white.png", "whitetr");
}

void TitleScene::Update()
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

	timer++;

	titleSpr.position = Float3{ 640.0f + xPos, 335.0f + sinf(timer * PI / 120) * 25 ,0 };
	tut1.position = Float3{ 1280.0f + 640 + xPos, 360, 0 };

	tut2.position = Float3{ 2560.0f + 640 + xPos, 360, 0 };

	tut3.position = Float3{ 2560.0f + 1280.0f + 640 + xPos, 360, 0 };

	titleSpr.UpdateMatrix();

	tut1.UpdateMatrix();
	tut2.UpdateMatrix();
	tut3.UpdateMatrix();

	if (tutorialTimer)
	{
		if (tutorialTimer < 65)
		{
			xPos -= 20;
			tutorialTimer++;
		}
		else
		{
			tutorialTimer = 0;
			tutorialMoving = false;
		}
	}

	if (xPos <= -1280 * 4)
	{
		bgm->Stop();
		this->outTran = true;
	}

	if ((Input::Pad::DownAny()||Input::Key::Triggered(DIK_SPACE)) && !tutorialMoving)
	{
		tutorialTimer++;
		tutorialMoving = true;
	}

	backPos -= 1;
	if (backPos <= -640) {
		backPos = 640;
	}

	back1.position.x = backPos;
	back2.position.x = backPos + 1280;

	back1.UpdateMatrix();
	back2.UpdateMatrix();
}

void TitleScene::DrawBack()
{
	back1.Draw();
	back2.Draw();
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawSprite()
{
	titleSpr.Draw();
	tut1.Draw();
	tut2.Draw();
	tut3.Draw();

	if (inTran || outTran)
	{
		white.position = { 640, 360, 0 };
		white.scale = { 2000, 2000, 1 };
		white.brightness = tranColor;
		white.UpdateMatrix();
		white.Draw();
	}
}
