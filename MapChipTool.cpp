#include "stdafx.h"
#include "MapChipTool.h"
#include <Input.h>
#include <MapChip.h>
#include <Camera2D.h>
#include <SpImGui.h>

void MapChipTool::Update()
{
	if (Input::Mouse::Down(Click::Left))
	{
		int x, y;

		Float2 mouse = Input::Mouse::GetPos();
		x = ((int)(mouse.x + Camera2D::Get()->x)) / MapChip::tileSize;
		y = ((int)(mouse.y + Camera2D::Get()->y)) / MapChip::tileSize;

		if (x >= 0 && x < MapChip::mapSizeX && y >= 0 && y < MapChip::mapSizeY)
		{
			MapChip::map.at(y).at(x) = chipNum;
		};
	}

	if (Input::Mouse::Down(Click::Right))
	{
		int x, y;

		Float2 mouse = Input::Mouse::GetPos();
		x = ((int)(mouse.x + Camera2D::Get()->x)) / MapChip::tileSize;
		y = ((int)(mouse.y + Camera2D::Get()->y)) / MapChip::tileSize;

		if (x >= 0 && x < MapChip::mapSizeX && y >= 0 && y < MapChip::mapSizeY)
		{
			MapChip::map.at(y).at(x) = 0;
		};
	}
}

void MapChipTool::Draw()
{
	SpImGui::Command([&] {
		if (ImGui::Begin("MapChip Tool")) {
			ImGui::InputInt("Map Chip Number", &chipNum);
		}
		ImGui::End();
	});
}

int MapChipTool::chipNum = 1;