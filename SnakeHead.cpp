#include "stdafx.h"
#include "SnakeHead.h"
#include <SpDS.h>
#include <MapChip.h>
#include <SpImGui.h>
#include <Input.h>

SnakeHead::SnakeHead(int x, int y, int id)
{
	this->x = x;
	this->y = y;
	this->id = id;
}

void SnakeHead::Draw()
{
	SpDS::DrawBox(x, y, MapChip::tileSize, MapChip::tileSize, 0, Color(0xdd3333));

	if (showImGui)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Snake Head"))
			{
				ImGui::InputInt2("Pos", &x);
				ImGui::InputInt("ID", &id);
				if (ImGui::ColorButton("Close", {0.8f, 0.2f, 0.2f, 1.0f}))
				{
					showImGui = false;
				}
			}
			ImGui::End();
		});
	}
}

void SnakeHead::OnClick()
{
	showImGui = true;
}

void SnakeHead::DrawAll()
{
	for (auto& h : heads) {
		h.Draw();
	}
}

void SnakeHead::OnClickSt()
{
	for (auto& h : heads) {
		Float2 mp = Input::Mouse::GetPos();
		if (abs(h.x - mp.x) < MapChip::tileSize / 2 && abs(h.y - mp.y) < MapChip::tileSize / 2)
		{
			h.OnClick();
		}
	}
}
