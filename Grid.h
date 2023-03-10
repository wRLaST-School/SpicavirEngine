#pragma once
class Grid
{
public:
	static enum class GridMode {
		Back,
		Trasparent,
		Front,
		Disable
	} gridMode;

	static void Draw();
	static void DrawOver();
private:
	
};

