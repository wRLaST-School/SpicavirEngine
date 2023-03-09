#include "stdafx.h"
#include "ToolManager.h"

void ToolManager::SetTool(Tool name)
{

	switch (name) {
		//‘I‚Î‚ê‚½ƒc[ƒ‹‚É‚æ‚Á‚Ä•ªŠò
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