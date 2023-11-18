#include "stdafx.h"
#include "TitleScene.h"
#include <Input.h>
#include <Transition.h>
#include <GameScene.h>
#include <SpDS.h>
#include <SpImGui.h>
#include <GameManager.h>

void TitleScene::LoadResources()
{
	SpTextureManager::LoadTexture("Assets/Images/title.png", "titleText");
}

void TitleScene::Init()
{
	Camera::Set(cam);
}

void TitleScene::Update()
{
	if (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A))
	{
		Transition::Start<GameScene>();
	}
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawSprite()
{
	Float2 center = { (float)GetSpWindow()->width / 2, (float)GetSpWindow()->height / 2 };
	Float2 multiplier = { center.x / 960, center.y / 540 };

	SpDS::DrawBox(0, 0, (int32_t)center.x * 2, (int32_t)center.y * 2, Color::Black);
	SpDS::DrawRotaGraph((int32_t)center.x, (int32_t)center.y, multiplier.x, multiplier.y, 0.f, "titleText");

	if (GameManager::sShowDebug)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Game Scene"))
			{
				ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
				ImGui::Checkbox("Show Hit Box", &GameManager::sShowHitBoxes);
				ImGui::Checkbox("Debug Immunity", &GameManager::sDebugImmunity);
				ImGui::Checkbox("Pause Game", &GameManager::sDebugTimeStop);
			}
			ImGui::End();
			});
	}
}
