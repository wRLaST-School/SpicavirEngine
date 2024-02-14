#include "TestPlayer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <format>
#include <functional>

std::function<void(int32_t, int32_t, float, float, float, std::string)> DrawRotaGraph;

void TestPlayer::Init()
{
	OutputDebugStringA("TestPlayer Initialize\n");
}

void TestPlayer::Update()
{
	if (count < 30)
	{
		std::string c = std::format("TestPlayer Update {}\n", count);
		OutputDebugStringA(c.c_str());
	}
}

void TestPlayer::Draw()
{
	if (count < 30)
	{
		std::string c = std::format("TestPlayer Draw {}\n", count);
		OutputDebugStringA(c.c_str());
	}

	if (DrawRotaGraph)
	{
		DrawRotaGraph(500, 500, 1.f, 1.f, 0.f, "titleText");
	}

	count++;
}

void SetDrawSpriteFunc(void(*dsfunc)(int32_t, int32_t, float, float, float, std::string))
{
	DrawRotaGraph = dsfunc;
}

TestPlayer* Create()
{
	return new TestPlayer();
}
