#include "stdafx.h"
#include "MoveBlockTool.h"
#include <Input.h>
#include <Camera2D.h>
#include <SpImGui.h>
#include <MapChip.h>


void MoveBlockTool::Update()
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

		MoveBlock::blocks.emplace_back(snappedX, snappedY, dir, width, height);
	}

	if (Input::Mouse::Triggered(Click::Right))
	{
		MoveBlock::OnClickSt();
	}
}

void MoveBlockTool::Draw()
{
	SpImGui::Command([&] {
		if (ImGui::Begin("MoveBlock Tool")) {
			ImGui::InputInt2(u8"サイズ", &width);
			ImGui::Combo(u8"方向", (int*)&dir, u8"上\0下\0左\0右\0\0");
		}
	ImGui::End();
	});
}
