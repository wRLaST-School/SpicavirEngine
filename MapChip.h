#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>
class MapChip
{
public:
	static void LoadResource();

	static void Init(int sizeX, int sizeY);
	static void Resize(int newX, int newY);

	static void Draw();
	static vector<vector<int>>map;
	static const int tileSize = 48;
	static const float tileMult;
	static int mapSizeX;
	static int mapSizeY;
	static vector<TextureKey> mapKeys;

	static int newx, newy;

	static bool showSizeImGui;
};