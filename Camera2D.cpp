#include "stdafx.h"
#include "Camera2D.h"

void Camera2D::DrawRotaGraphCam(int x, int y, float dx, float dy, float rot, TextureKey key, Anchor anchor, Color color)
{
	SpDS::DrawRotaGraph(x - this->x, y - this->y, dx, dy, rot, key, anchor, color);
}

Camera2D* Camera2D::Get()
{
	return current;
}

void Camera2D::Set(Camera2D* camera)
{
}

Camera2D* Camera2D::current = nullptr;