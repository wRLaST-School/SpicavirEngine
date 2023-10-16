#include "stdafx.h"
#include "SrSpriteStage.h"
#include <Sprite.h>
#include <SpDS.h>
#include <DockPanel.h>

void SrSpriteStage::DrawCommands(std::function<void(void)> cmd)
{
	commands_.push_back(cmd);
}

void SrSpriteStage::Init()
{
}

void SrSpriteStage::PreDraw()
{
	Sprite::PreSpriteDraw();
}

void SrSpriteStage::PostDraw()
{
	Sprite::PostSpriteDraw();

	DockPanel::DrawViewPort();
}

void SrSpriteStage::Render()
{
	for (auto& cmd : commands_)
	{
		cmd();
	}
	commands_.clear();

	SpDS::Render();
}
