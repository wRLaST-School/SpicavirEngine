#include "stdafx.h"
#include "SnakeBody.h"
#include <MapChip.h>
#include <Input.h>
#include <SpDS.h>
#include <SpImGui.h>
#include <Camera2D.h>
vector<SnakeBody> SnakeBody::bodies;
SnakeBody::SnakeBody(int x, int y, int id, int order):x(x), y(y), id(id), order(order)
{
}
void SnakeBody::Draw()
{
	SpDS::DrawBox(x - Camera2D::Get()->x, y - Camera2D::Get()->y, MapChip::tileSize, MapChip::tileSize, 0, Color(0x33dddd));

	if (showImGui)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Snake Body"))
			{
				ImGui::InputInt2(u8"À•W", &x);
				ImGui::InputInt(u8"ID", &id);
				ImGui::InputInt(u8"‡”Ô", &order);
				if (ImGui::Button(u8"•Â‚¶‚é"))
				{
					showImGui = false;
				}
				if (ImGui::Button(u8"íœ"))
				{
					for(auto itr = bodies.begin(); itr != bodies.end(); itr ++)
						if (&(*itr) == this) {
							SnakeBody::bodies.erase(itr);
							break;
						}
				}
				ImGui::Checkbox("IS CLIENT ACTIVE", &GetSpWindow()->isClientActive);
			}
			ImGui::End();
		});
	}
}

void SnakeBody::OnClick()
{
	for (auto& b : bodies) b.showImGui = false;
	showImGui = true;
}

void SnakeBody::DrawAll()
{
	for (auto& b : bodies) {
		b.Draw();
	}
}

void SnakeBody::Init()
{
	bodies.clear();
}

void SnakeBody::OnClickSt()
{
	for (auto& b : bodies) {
		Float2 mp = Input::Mouse::GetPos();
		if (abs(b.x - (mp.x + Camera2D::Get()->x)) < MapChip::tileSize / 2 && abs(b.y - (mp.y + Camera2D::Get()->y)) < MapChip::tileSize / 2)
		{
			b.OnClick();
		}
	}
}
