#pragma once
#include <SpTextureManager.h>
class Player
{
public:
	Player() {};
	void Update();
	void Draw();

	Vec2 pos;
	Vec2 size;

	TextureKey tex = "Player";

	float speed = 8.0f;

public:
	static void Load();
	static void Init();
	static Player* Get();

	static Player current;
};

