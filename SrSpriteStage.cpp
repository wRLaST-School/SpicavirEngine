#include "stdafx.h"
#include "SrSpriteStage.h"
#include <Sprite.h>
#include <SpDS.h>

void SrSpriteStage::DrawCommands(std::function<void(void)> cmd)
{
	commands.push_back(cmd);
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
}

void SrSpriteStage::Render()
{
	for (auto cmd : commands)
	{
		cmd();
	}
	commands.clear();

	SpDS::Render();
}
