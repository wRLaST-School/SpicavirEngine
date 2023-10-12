#include "stdafx.h"
#include "Score.h"

#include "Score.h"
#include <SpDS.h>
#include <Util.h>

void Score::Load()
{
	SpTextureManager::LoadTexture("Assets/Images/ScoreBase.png", "scoreBase");
	SpTextureManager::LoadTexture("Assets/Images/ScoreTierOverlay.png", "scoreOverlay");
}

void Score::Init()
{
	totDamage = 0;
}

void Score::Update()
{
}

void Score::Draw()
{
	Float4 gColor = { 1,1,1,1 };

	if (totDamage < gradeC)
	{
		gColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	}
	else if (totDamage < gradeB)
	{
		gColor = { 0.5f, 0.5f, 0.5f, 1.0f };

	}
	else if (totDamage < gradeA)
	{
		gColor = { 0.7f, 0.7f, 0.7f, 1.0f };

	}
	else if (totDamage < gradeS)
	{
		gColor = { 0.85f, 0.85f, 0.85f, 1.0f };
	}

	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, 32, 1.f, 1.f, 0.f, "scoreBase");
	SpDS::DrawBox(GetSpWindow()->width / 2 - 504, 8, GetSpWindow()->width / 2 - 504 + (int)Util::ClampMax((float)1008 * (float)totDamage / (float)gradeMax, 1008.f), 56, Color(gColor));
	SpDS::DrawRotaGraph(GetSpWindow()->width / 2, 32, 1.f, 1.f, 0.f, "scoreOverlay");
}