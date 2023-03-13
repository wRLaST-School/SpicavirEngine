#pragma once
#include <Essentials.h>

class SnakeHead
{
public:
	int x, y;
	int id;

	void Draw();

	void OnClick();

	static void DrawAll();

	static void OnClickSt();

private:
	static vector<SnakeHead> heads;
};

