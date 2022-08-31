#pragma once
#include "Object3D.h"
#include "Sprite.h"
class Marker
{
public:
	static Float3 MonitorToWorld(Float3 monitor);
	static Float3 WorldToMonitor(Float3 world);
	static void SetMarkerTextures(TextureKey outer, TextureKey inner);

	Marker(Float3 moniPos);
	
	//delのフラグを建てるため削除チェック→Updateの順で呼ぶ
	void Update();

	void Draw();

	Float3 pos;
	bool del = false;

private:
	Sprite outerSpr;
	Sprite innerSpr;

	int timer = 0;
	const int finTime = 60;

	static TextureKey outertex;
	static TextureKey innertex;

	int r = 50;
	//200x200;
	int spriteR = 100;
};

