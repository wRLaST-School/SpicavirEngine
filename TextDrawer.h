#pragma once
#include "Sprite.h"
class TextDrawer
{
private:
	static const int maxChar = 256;
	static const int fontWidth = 64;
	static const int fontHeight = 128;
	static const int charsPerLine = 14;

public:
	Sprite sprites[maxChar];
	int nextSpriteIndex = 0;

	static void Init();

private:
};

