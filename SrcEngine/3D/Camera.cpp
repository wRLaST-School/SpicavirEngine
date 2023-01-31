#include "Camera.h"

#include <SpDirectX.h>

Camera::Camera()
{
	renderWidth = 0;
	renderHeight = 0;
}

void Camera::SetRenderSize(float w, float h)
{
	renderWidth = w;
	renderHeight = h;
}

void Camera::UseDefaultParams()
{
	useWindowSize = true;
	nearZ = .1f;
	farZ = 1000.f;

	position = { 0.f, 0.f, -10.f };
	rotation = { 0.f, 0.f, 0.f };

	fov = PIf / 2;
	nearZ = 0.1f;
	farZ = 1000.0f;
}

Matrix Camera::GetBillboardMat()
{
	Matrix camRot = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());
	camRot[3] = { 0,0,0,1 };

	return -camRot;
}

void Camera::Set(Camera& camera)
{
	current = &camera;
}

void Camera::UseCurrent()
{
	if (current->useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		current->renderWidth = (float)wnd->width;
		current->renderHeight = (float)wnd->height;
	}

	D3D12_VIEWPORT viewport{};

	viewport.Width = current->renderWidth;
	viewport.Height = current->renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + (LONG)current->renderWidth;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + (LONG)current->renderHeight;       // 切り抜き座標下

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	Matrix vMat = current->targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(current->position, current->target, current->matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(current->position, current->matWorld.ExtractAxisZ(), current->matWorld.ExtractAxisY());

	Matrix pMat = current->projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(current->fov, (float)current->renderWidth / (float)current->renderHeight, current->nearZ, current->farZ) :
		Matrix::ProjectionOrtho((int)current->renderWidth, (int)current->renderHeight, current->nearZ, current->farZ, 20);

	current->cameraViewProjMatrixCB.contents->vproj = vMat * pMat;
	current->cameraViewProjMatrixCB.contents->cameraPos = current->position;

	current->cameraViewProjMatrixCB.contents->billboardMat = GetCurrentCameraBillboardMat();

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(3, current->cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());
}

Matrix Camera::GetCurrentCameraBillboardMat()
{
	Matrix camRot = current->targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(current->position, current->target, current->matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(current->position, current->matWorld.ExtractAxisZ(), current->matWorld.ExtractAxisY());
	camRot[3] = { 0,0,0,1 };

	return -camRot;
}

Camera* Camera::current = nullptr;
