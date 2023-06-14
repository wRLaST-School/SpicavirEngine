#include "stdafx.h"
#include "Camera2D.h"

void Camera2D::DrawRotaGraphCam(int32_t cx, int32_t cy, float dx, float dy, float rot, const TextureKey& key, const Anchor& anchor, const Color& color)
{
	SpDS::DrawRotaGraph(cx - this->x, cy - this->y, dx, dy, rot, key, anchor, color);
}

Camera2D* Camera2D::Get()
{
	return sCurrent;
}

void Camera2D::Set(Camera2D* camera)
{
	sCurrent = camera;
}

Matrix Camera2D::GetViewProjMat()
{
	Matrix vMat = Matrix::View(Matrix::Translation({ (float)sCurrent->x, (float)sCurrent->y, 0.f }));

	SpWindow* win = GetSpWindow();
	Matrix pMat = Matrix::ProjectionOrtho(win->width, win->height, -1.f, 1.f);

	return vMat * pMat;
}

Camera2D* Camera2D::sCurrent = nullptr;