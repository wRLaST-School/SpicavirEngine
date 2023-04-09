#include "stdafx.h"
#include "MapChip.h"
#include <Camera2D.h>
#include <SpImGui.h>

vector<vector<int>> MapChip::map;

int MapChip::mapSizeX = 0;
int MapChip::mapSizeY = 0;
const float MapChip::tileMult = (float)tileSize / 60.f;

void MapChip::LoadResource()
{
	vector<TextureKey> mapKeyst = {
		"RedSolid",
		"BlueSolid",
		"RedOutline",
		"BlueOutline",
		"BlackSolid",
		"Trapfall"
	};

	SpTextureManager::LoadDiv("Resources/mapchip.png", 60, 60, 6, 1, mapKeyst);

	mapKeys.push_back("RedSolid");
	mapKeys.push_back("BlueSolid");

	mapKeys.push_back("RedOutline");
	mapKeys.push_back("BlueOutline");
	mapKeys.push_back("RedOutline");

	mapKeys.push_back("BlackSolid");
	mapKeys.push_back("Trapfall");
}

void MapChip::Init(int sizeX, int sizeY)
{
	map.clear();
	for (int y = 0; y < sizeY; y++)
	{
		map.push_back(vector<int>());
		for (int x = 0; x < sizeX; x++)
		{
			map.back().push_back(0);
		}
	}

	mapSizeX = sizeX;
	mapSizeY = sizeY;
}

void MapChip::Resize(int newX, int newY)
{
	if (newX > mapSizeX)
	{
		for (auto& r : map)
		{
			for (int i = 0; i < newX - mapSizeX; i++)
			{
				r.push_back(0);
			}
		}
	}

	else if (newX < mapSizeX)
	{
		for (auto& r : map)
		{
			for (int i = 0; i < mapSizeX - newX; i++)
			{
				r.pop_back();
			}
		}
	}

	mapSizeX = newX;

	if (newY > mapSizeY)
	{
		for (int y = 0; y < newY - mapSizeY; y++)
		{
			map.push_back(vector<int>());
			for (int x = 0; x < mapSizeX; x++)
			{
				map.back().push_back(0);
			}
		}
	}

	if (newY < mapSizeY)
	{
		for (int y = 0; y < mapSizeY - newY; y++)
		{
			map.pop_back();
		}
	}

	mapSizeY = newY;
}

void MapChip::Draw()
{
	Camera2D* cam = Camera2D::Get();
	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			if (map.at(y).at(x))
			{
				SpDS::DrawRotaGraph(x * tileSize - cam->x, y * tileSize - cam->y, tileMult, tileMult, 0, mapKeys.at(map.at(y).at(x) - 1), Anchor::TopLeft, Color(0xffffff));
			}
		}
	}

	if (showSizeImGui) {
		SpImGui::Command([&] {
			ImGui::SetNextWindowPos(ImVec2(GetSpWindow()->width / 2, GetSpWindow()->height / 2));
			if (ImGui::Begin(u8"マップサイズ")) {
				ImGui::InputInt(u8"幅", &newx);
				ImGui::InputInt(u8"高さ", &newy);

				if (ImGui::Button(u8"決定")) {
					Resize(newx, newy);
					showSizeImGui = false;
				}

				if (ImGui::Button(u8"キャンセル")) {
					newx = mapSizeX;
					newy = mapSizeY;
					showSizeImGui = false;
				}
			}

			ImGui::End();
		});
	}
}

vector<TextureKey> MapChip::mapKeys;
bool MapChip::showSizeImGui = false;
int MapChip::newx = 32, MapChip::newy = 18;