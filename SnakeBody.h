#pragma once
#include <Essentials.h>
class SnakeBody
{
public:
	int x, y;
	int id;
	int order;
	bool showImGui = false;

	SnakeBody(int x, int y, int id, int order);

	void Draw();

	void OnClick();

	static void DrawAll();
	static void Init();
	static void OnClickSt();
	static vector<SnakeBody> bodies;

private:
};

