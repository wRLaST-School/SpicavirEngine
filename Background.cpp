#include "stdafx.h"
#include "Background.h"
#include <SpWindow.h>
#include <SpDS.h>

long long Background::index = 0;

void Background::Load()
{
	key = SpTextureManager::LoadTexture("Resources/Back.png", string("back") + to_string(index));
	index++;
	tileSize = SpTextureManager::GetTextureMetadata(key).width;
}

void Background::Init()
{
}

void Background::Update()
{
}

void Background::Draw()
{
	int wx = GetSpWindow()->width;
	int wy = GetSpWindow()->height;

	//画面を覆うサイズまでタイル敷き詰め
	for(int i = 0; i <= wx; i += tileSize)
		for(int j = 0; j <= wy; j += tileSize)
			SpDS::DrawRotaGraph(i, j, 1.f, 1.f, 0.f, key, Anchor::TopLeft);
}
