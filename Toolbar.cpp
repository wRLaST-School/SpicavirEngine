#include "stdafx.h"
#include "Toolbar.h"
#include <SpImGui.h>
#include <SpWindow.h>
#include <FileSaver.h>
#include <resource3.h>
#include <ToolManager.h>
#include <MapChip.h>

HMENU Toolbar::hm;

void Toolbar::ProcessMessage(WPARAM wp)
{
	switch (wp) {
		case ID_NEWFILE:
		{
			MapChip::Init(32, 18);
			return;
		};

		case ID_OPEN:
		{
			FileSaver::Open();
			return;
		}

		case ID_NEWSAVE:
		{
			FileSaver::Save();
			return;
		}

		case ID_OVERWRITE:
		{
			FileSaver::Overwrite();
			return;
		}

		case ID_QUIT:
		{
			SendMessage(GetSpWindow()->hwnd, WM_CLOSE, NULL, NULL);
			return;
		}

		case ID_CHIP1:
		{
			ToolManager::SetMapTool(1);
			return;
		}
		case ID_CHIP2:
		{
			ToolManager::SetMapTool(2);
			return;
		}
		case ID_CHIP3:
		{
			ToolManager::SetMapTool(3);
			return;
		}
		case ID_CHIP4:
		{
			ToolManager::SetMapTool(4);
			return;
		}
		case ID_CHIP5:
		{
			ToolManager::SetMapTool(5);
			return;
		}
		case ID_CHIP6:
		{
			ToolManager::SetMapTool(6);
			return;
		}
		case ID_CHIP7:
		{
			ToolManager::SetMapTool(7);
			return;
		}
		case ID_CHIP8:
		{
			ToolManager::SetMapTool(8);
			return;
		}
		case ID_CHIP9:
		{
			ToolManager::SetMapTool(9);
			return;
		}
		case ID_CHIP10:
		{
			ToolManager::SetMapTool(10);
			return;
		}
		case ID_CHIP11:
		{
			ToolManager::SetMapTool(11);
			return;
		}
		case ID_CHIP12:
		{
			ToolManager::SetMapTool(12);
			return;
		}
		case ID_CHIP13:
		{
			ToolManager::SetMapTool(13);
			return;
		}
		case ID_CHIP14:
		{
			ToolManager::SetMapTool(14);
			return;
		}
		case ID_CHIP15:
		{
			ToolManager::SetMapTool(15);
			return;
		}
		case ID_CHIP16:
		{
			ToolManager::SetMapTool(16);
			return;
		}
	}
}
