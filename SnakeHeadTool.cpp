#include "stdafx.h"
#include "SnakeHeadTool.h"
#include <Input.h>
#include <SnakeHead.h>
#include <Camera2D.h>
#include <MapChip.h>
#include <SpImGui.h>

void SnakeHeadTool::Update()
{
	if (Input::Mouse::Triggered(Click::Left))
	{
		int snappedX, snappedY;
		int chipX, chipY;

		Float2 mouse = Input::Mouse::GetPos();
		chipX = ((int)(mouse.x + Camera2D::Get()->x)) / MapChip::tileSize;
		chipY = ((int)(mouse.y + Camera2D::Get()->y)) / MapChip::tileSize;

		snappedX = ((float)chipX + 0.5f) * MapChip::tileSize;
		snappedY = ((float)chipY + 0.5f) * MapChip::tileSize;

		SnakeHead::heads.emplace_back(snappedX, snappedY, id);

		id++;
	}

	if (Input::Mouse::Triggered(Click::Right))
	{
		SnakeHead::OnClickSt();
	}
}

void SnakeHeadTool::Draw()
{
	SpImGui::Command([&] {
		if (ImGui::Begin("SnakeHead Tool")) {
			ImGui::InputInt("Snake ID", &id);
		}
		ImGui::End();
	});
}
