#include "stdafx.h"
#include "Toolbar.h"
#include <SpImGui.h>
#include <SpWindow.h>
#include <FileSaver.h>
#include <resource3.h>

HMENU Toolbar::hm;

void Toolbar::ProcessMessage(WPARAM wp)
{
	switch (wp) {
		case ID_NEWFILE:
		{
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
	}
}
