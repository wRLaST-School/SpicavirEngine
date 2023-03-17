#pragma once
#include <Essentials.h>
#include <IToolMode.h>
class ToolManager
{
public:
	enum class Tool {
		SnakeHead,
		SnakeBody,
		MoveBlock
	};
	static void SetTool(Tool name);
	static void SetMapTool(int chipID);
	static void Update();
	static void Draw();

	static unique_ptr<IToolMode> tool;
	static bool isMouseActive;
};

