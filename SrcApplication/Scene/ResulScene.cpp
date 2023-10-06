#include "stdafx.h"
#include "ResulScene.h"
#include <Input.h>
#include <Transition.h>
#include <TitleScene.h>
#include <GameManager.h>
#include <SpDS.h>

void ResultScene::LoadResources()
{
	if (GameManager::sScore.gradeMax > GameManager::sScore.totDamage)
	{
		SpTextureManager::LoadTexture("Resources/result.png", "resultText");

		if (GameManager::sScore.totDamage > GameManager::sScore.gradeS)
		{
			SpTextureManager::LoadTexture("Resources/rankS.png", "rank");
		}
		else if (GameManager::sScore.totDamage > GameManager::sScore.gradeA)
		{
			SpTextureManager::LoadTexture("Resources/rankA.png", "rank");
		}
		else if (GameManager::sScore.totDamage > GameManager::sScore.gradeB)
		{
			SpTextureManager::LoadTexture("Resources/rankB.png", "rank");
		}
		else if (GameManager::sScore.totDamage > GameManager::sScore.gradeC)
		{
			SpTextureManager::LoadTexture("Resources/rankC.png", "rank");
		}
		else
		{
			SpTextureManager::LoadTexture("Resources/rankD.png", "rank");
		}
	}
	else
	{
		SpTextureManager::LoadTexture("Resources/clearResult.png", "resultText");
	}
}

void ResultScene::Init()
{
	Camera::Set(cam);
}

void ResultScene::Update()
{
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		Transition::Start<TitleScene>();
	}
}

void ResultScene::DrawBack()
{
}

void ResultScene::Draw3D()
{
}

void ResultScene::DrawSprite()
{
	Float2 center = { (float)GetSpWindow()->width /2, (float)GetSpWindow()->height /2 };
	Float2 multiplier = { center.x / 640, center.y / 360 };

	SpDS::DrawBox(0, 0, (int32_t)center.x * 2, (int32_t)center.y * 2, Color::Black);
	SpDS::DrawRotaGraph((int32_t)center.x, (int32_t)center.y, multiplier.x, multiplier.y, 0.f, "resultText");
	SpDS::DrawRotaGraph((int32_t)center.x, (int32_t)center.y, multiplier.x, multiplier.y, 0.f, "rank");
}