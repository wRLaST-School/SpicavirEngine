#include "stdafx.h"
#include "ToolManager.h"
#include <MapChipTool.h>
#include <SnakeHeadTool.h>
#include <SnakeBodyTool.h>
#include <MoveBlockTool.h>
#include <SpWindow.h>
#include <Input.h>
bool ToolManager::isMouseActive = true;
void ToolManager::SetTool(Tool name)
{
	switch (name) {//‘I‚Î‚ê‚½ƒc[ƒ‹‚É‚æ‚Á‚Ä•ªŠò
		case Tool::SnakeHead:
			tool = unique_ptr<IToolMode>(new SnakeHeadTool());
			break;

		case Tool::SnakeBody:
			tool = unique_ptr<IToolMode>(new SnakeBodyTool());
			break;

		case Tool::MoveBlock:
			tool = unique_ptr<IToolMode>(new MoveBlockTool());
			break;
	};
}

void ToolManager::SetMapTool(int chipID)
{
	tool = unique_ptr<IToolMode>(new MapChipTool(chipID));
}

void ToolManager::Update()
{
	if (!GetSpWindow()->isClientActive)
	{
		bool isMouseActive = false;
	}
	if (Input::Mouse::Released(Click::Left))
	{
		bool isMouseActive = true;
	}

	tool->Update();
}

void ToolManager::Draw()
{
	tool->Draw();
}

unique_ptr<IToolMode> ToolManager::tool = unique_ptr<IToolMode>(new MapChipTool(0));