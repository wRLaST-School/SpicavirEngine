#include "stdafx.h"
#include "MoveBlock.h"
#include <SpDS.h>
#include <SpImGui.h>
#include <Camera2D.h>
#include <MapChip.h>
#include <Input.h>
vector<MoveBlock> MoveBlock::blocks;
MoveBlock::MoveBlock(int x, int y, Direction dir, int width, int height) : x(x), y(y),dir(dir), width(width), height(height)
{
}

void MoveBlock::Draw()
{
	SpDS::DrawBox(x - Camera2D::Get()->x, y - Camera2D::Get()->y, width, height, 0, Color(0xdddddd));

	if (showImGui)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Snake Body"))
			{
				ImGui::InputInt2(u8"座標", &x);
				ImGui::InputInt(u8"方向", (int*)&dir);
				ImGui::InputInt2(u8"サイズ", &width);
				if (ImGui::Button(u8"閉じる"))
				{
					showImGui = false;
				}
				if (ImGui::Button(u8"削除"))
				{
					for (auto itr = blocks.begin(); itr != blocks.end(); itr++)
						if (&(*itr) == this) {
							MoveBlock::blocks.erase(itr);
							break;
						}
				}
			}
		ImGui::End();
			});
	}
}

void MoveBlock::OnClick()
{
	for (auto& b : blocks) b.showImGui = false;
	showImGui = true;
}

void MoveBlock::DrawAll()
{
	for (auto& b : blocks) {
		b.Draw();
	}
}

void MoveBlock::Init()
{
	blocks.clear();
}

void MoveBlock::OnClickSt()
{
	for (auto& b : blocks) {
		Float2 mp = Input::Mouse::GetPos();
		if (abs(b.x - mp.x) < b.width / 2 && abs(b.y - mp.y) < b.height/ 2)
		{
			b.OnClick();
		}
	}
}
