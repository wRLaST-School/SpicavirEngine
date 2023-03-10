#pragma once
class Grid
{
public:
	static enum class GridMode {
		Back,
		Trasparent,
		Front
	} gridMode;

	static void Draw();
	static void DrawOver();
private:
	
};

