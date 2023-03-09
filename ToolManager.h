#pragma once
#include <Essentials.h>
#include <MapChipTool.h>
class ToolManager
{
public:
	enum class Tool {

	};
	static void SetTool(Tool name);
	static void SetMapTool(int chipID);
	static void Update();

	static unique_ptr<IToolMode> tool;
};

