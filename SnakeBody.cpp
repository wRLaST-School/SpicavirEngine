#include "stdafx.h"
#include "SnakeBody.h"
#include <MapChip.h>
#include <Input.h>
#include <SpDS.h>
#include <SpImGui.h>

void SnakeBody::Draw()
{
	SpDS::DrawBox(x, y, MapChip::tileSize, MapChip::tileSize, 0, Color(0x33dddd));

	if (showImGui)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Snake Body"))
			{
				ImGui::InputInt2("Pos", &x);
				ImGui::InputInt("ID", &id);
				ImGui::InputInt("Order", &order);
				if (ImGui::ColorButton("Close", { 0.8f, 0.2f, 0.2f, 1.0f }))
				{
					showImGui = false;
				}
			}
			ImGui::End();
			});
	}
}

void SnakeBody::OnClick()
{
	showImGui = true;
}

void SnakeBody::DrawAll()
{
	for (auto& b : bodies) {
		b.Draw();
	}
}

void SnakeBody::OnClickSt()
{
	for (auto& b : bodies) {
		Float2 mp = Input::Mouse::GetPos();
		if (abs(b.x - mp.x) < MapChip::tileSize / 2 && abs(b.y - mp.y) < MapChip::tileSize / 2)
		{
			b.OnClick();
		}
	}
}
