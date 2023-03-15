#pragma once
#include <Essentials.h>
class MoveBlock
{
public:
	int x, y;
	int width, height;
	bool showImGui = false;

	enum class Direction {
		Up,
		Down,
		Left,
		Right
	} dir;

	MoveBlock(int x, int y, Direction dir, int width, int height);

	void Draw();

	void OnClick();

	static void DrawAll();
	static void Init();
	static void OnClickSt();
	static vector<MoveBlock> blocks;

private:
};

