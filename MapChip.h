#pragma once
#include <SpTextureManager.h>
enum class MapType {
	Air,
	Solid,
	Platform
};

struct MapData {
	MapType type;
	TextureKey key;
};

static class MapChip
{
public:
	static void Load();

	static void Init();
	static void Draw();

	static eastl::vector<eastl::vector<MapData>> mapdata;
private:
	const static MapData mapDataTable[];
	const static int tileSize = 60;
};

