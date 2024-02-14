#include "TestPlayer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <format>
#include <string>

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

	count++;
}

TestPlayer* Create()
{
	return new TestPlayer();
}
