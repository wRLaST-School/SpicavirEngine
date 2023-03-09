#pragma once
#include <Essentials.h>
class MapChip
{
public:
	static void LoadResource();

	static void Init(int sizeX, int sizeY);
	static void Resize(int newX, int newY);

	static void Draw();
	static vector<vector<int>>map;
	static const int tileSize = 48;
	static int mapSizeX;
	static int mapSizeY;
};