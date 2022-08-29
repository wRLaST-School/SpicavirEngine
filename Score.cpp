#include "Score.h"

void Score::Init()
{
	gauge = Sprite("white");
	gaugeBG = Sprite("Resources/ScoreBase.png", "scorebase");
	gaugeOverlay = Sprite("Resources/ScoreTierOverlay.png", "scoreoverlay");

	gaugeOverlay.position = {(float)GetwWindow()->width / 2, 32.0f, 0.f};
	gaugeBG.position = gaugeOverlay.position;
}

void Score::Update()
{
	gauge.scale = { min((float)1008 * (float)totDamage / (float)gradeMax, 1008), 48, 1 };
	gauge.position = { gauge.scale.x / 2 + 136, 32, 0 };

	Float4 gColor = {1,1,1,1};

	if (totDamage < gradeC)
	{
		gColor = {0.3, 0.3, 0.3, 1.0};
	}
	else if (totDamage < gradeB)
	{
		gColor = { 0.5, 0.5, 0.5, 1.0 };

	}
	else if (totDamage < gradeA)
	{
		gColor = { 0.7, 0.7, 0.7, 1.0 };

	}
	else if (totDamage < gradeS)
	{
		gColor = { 0.85, 0.85, 0.85, 1.0 };
	}

	gauge.brightness = gColor;

	gauge.UpdateMatrix();

	gaugeOverlay.UpdateMatrix();
	gaugeBG.UpdateMatrix();
}

void Score::Draw()
{
	gaugeBG.Draw();
	gauge.Draw();
	gaugeOverlay.Draw();
}
