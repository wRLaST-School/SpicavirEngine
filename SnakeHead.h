#pragma once
#include <Essentials.h>

class SnakeHead
{
public:
	int x, y;
	int id;
	bool showImGui = false;
	
	SnakeHead(int x, int y, int id);

	void Draw();

	void OnClick();

	static void DrawAll();
	static void Init();
	static void OnClickSt();
	static SnakeHead* GetMouseTile();
	static vector<SnakeHead> heads;

private:
};

