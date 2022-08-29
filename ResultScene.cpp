#include "ResultScene.h"

void ResultScene::Init()
{
}

void ResultScene::Update()
{
}

void ResultScene::DrawBack()
{
}

void ResultScene::Draw3D()
{
}

void ResultScene::DrawSprite()
{
}

void ResultScene::GiveTransitionData(GameSceneOutputs data)
{
	this->score.totDamage = data.finalScore.totDamage;
}
