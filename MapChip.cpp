#include "stdafx.h"
#include "MapChip.h"
#include <Camera2D.h>

vector<vector<int>> MapChip::map;

int MapChip::mapSizeX = 0;
int MapChip::mapSizeY = 0;

void MapChip::LoadResource()
{
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
			map.push_back(vector<int>());
			for (int x = 0; x < mapSizeX; x++)
			{
				map.back().pop_back();
			}
		}
	}
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
				SpDS::DrawBox(x * tileSize - cam->x, y * tileSize - cam->y, tileSize, tileSize, 0, Color(0xffc7f5), Anchor::TopLeft);
			}
		}
	}
}
