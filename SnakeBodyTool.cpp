#include "stdafx.h"
#include "SnakeBodyTool.h"
#include <SpImGui.h>
#include <SnakeBody.h>
#include <Input.h>
#include <Camera2D.h>
#include <MapChip.h>
#include <SnakeHead.h>

void SnakeBodyTool::Update()
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

		SnakeBody::bodies.emplace_back(snappedX, snappedY, id, order);

		order++;
	}

	if (Input::Mouse::Triggered(Click::Right))
	{
		SnakeBody::OnClickSt();

		SnakeHead* h = SnakeHead::GetMouseTile();
		if (h) id = h->id;
	}
}

void SnakeBodyTool::Draw()
{
	SpImGui::Command([&] {
		if (ImGui::Begin("SnakeBody Tool")) {
			ImGui::InputInt("ID", &id);
			ImGui::InputInt(u8"èáî‘", &order);
		}
		ImGui::End();
	});
}
