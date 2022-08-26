#include "ResultScene.h"

void ResultScene::GiveTransitionData(GameSceneOutputs data)
{
	this->score.totDamage = data.finalScore.totDamage;
}
