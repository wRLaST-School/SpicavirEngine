#pragma once
#include <SpTextureManager.h>
class Background
{
public:
	void Load();
	void Init();
	void Update();
	void Draw();

	TextureKey key;
	int tileSize = 0;
private:
	static long long index;
};

