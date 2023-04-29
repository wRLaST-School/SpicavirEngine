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
			ImGui::Combo(u8"ツール", &chipNum, u8"消しゴム\0赤ブロック\0青ブロック\0バグブロック(使用禁止)\0青点線(使用禁止)\0赤点線(使用禁止)\0黒ブロック\0落とし穴\0Sideスタート\0Topスタート\0ゴール\0レーザー(上)\0レーザー(下)\0レーザー(左)\0レーザー(右)");
		}
		ImGui::End();
	});
}

int MapChipTool::chipNum = 1;