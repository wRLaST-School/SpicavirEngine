#include "TestPlayer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <format>
#include <functional>
#include <SpDS.h>
#include <Object3D.h>
#include <Input.h>
#include <ScriptComponent.h>

std::function<void(int32_t, int32_t, float, float, float, std::string)> DrawRotaGraph;

void TestPlayer::Init()
{
	OutputDebugStringA("TestPlayer Initialize\n");
}

void TestPlayer::Update()
{
	auto obj = This()->Parent()->CastTo<Object3D>();

	if (obj)
	{
		float move = (float)Input::Key::Down(DIK_UP) - (float)Input::Key::Down(DIK_DOWN);
		move *= 0.1f;

		obj->position.z += move;

		move = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));

		move *= 0.1f;
		
		obj->position.x += move;
	}
}

void TestPlayer::Draw()
{
	
}

void SetDrawSpriteFunc(void(*dsfunc)(int32_t, int32_t, float, float, float, std::string))
{
	DrawRotaGraph = dsfunc;
}
