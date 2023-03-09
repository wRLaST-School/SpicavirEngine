#include "stdafx.h"
#include "ToolManager.h"

void ToolManager::SetTool(Tool name)
{

	switch (name) {
		//選ばれたツールによって分岐
	};
}

void ToolManager::SetMapTool(int chipID)
{
	tool = unique_ptr<IToolMode>(new MapChipTool(chipID));
}

void ToolManager::Update()
{
	tool->Update();
}

unique_ptr<IToolMode> ToolManager::tool = unique_ptr<IToolMode>(new MapChipTool(0));