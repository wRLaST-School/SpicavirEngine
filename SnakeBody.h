#pragma once
#include <Essentials.h>
class SnakeBody
{
public:
	int x, y;
	int id;
	int order;
	bool showImGui = false;

	void Draw();

	void OnClick();

	static void DrawAll();

	static void OnClickSt();

private:
	static vector<SnakeBody> bodies;
};

