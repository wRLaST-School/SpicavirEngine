#include "stdafx.h"
#include "Grid.h"
#include <MapChip.h>
#include <Camera2D.h>

Grid::GridMode Grid::gridMode = Grid::GridMode::Trasparent;

void Grid::Draw()
{
	if (gridMode != GridMode::Back)
	{
		return;
	}


}

void Grid::DrawOver()
{
	if (gridMode == GridMode::Back)
	{
		return;
	}

	float opacity = 1.0f;

	if (gridMode == GridMode::Trasparent)
	{
		opacity = 0.3f;
	}

	for (int i = 0; i <= MapChip::mapSizeX; i++) {
		SpDS::DrawLine(i * MapChip::tileSize - Camera2D::Get()->x, -Camera2D::Get()->y,
			i * MapChip::tileSize - Camera2D::Get()->x, MapChip::mapSizeY * MapChip::tileSize - Camera2D::Get()->y,
			Color(1.f, 1.f, 1.f, opacity), 4
		);
	}

	for (int i = 0; i <= MapChip::mapSizeY; i++) {
		SpDS::DrawLine( -Camera2D::Get()->x, i * MapChip::tileSize - Camera2D::Get()->y,
			MapChip::mapSizeX * MapChip::tileSize - Camera2D::Get()->x, i * MapChip::tileSize - Camera2D::Get()->y,
			Color(1.f, 1.f, 1.f, opacity), 4
		);
	}
}
