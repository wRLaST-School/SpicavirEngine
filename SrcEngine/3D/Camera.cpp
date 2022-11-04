#include "Camera.h"

#include <SpDirectX.h>

Camera::Camera()
{
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

void Camera::Set(Camera& camera)
{
	if (camera.useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		camera.renderWidth = wnd->width;
		camera.renderHeight = wnd->height;
	}

	D3D12_VIEWPORT viewport{};

	viewport.Width = camera.renderWidth;
	viewport.Height = camera.renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + camera.renderWidth;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + camera.renderHeight;       // 切り抜き座標下

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	Matrix vMat = camera.targetMode == CameraTargetMode::LookAt ? 
		Matrix::ViewLookAt(camera.position, camera.target, camera.matWorld.ExtractAxisY()) : 
		Matrix::ViewLookTo(camera.position, camera.matWorld.ExtractAxisZ(), camera.matWorld.ExtractAxisY());

	Matrix pMat = camera.projectionMode == ProjectionMode::Perspective ? 
		Matrix::Projection(camera.fov, (float)camera.renderWidth / (float)camera.renderHeight, camera.nearZ, camera.farZ) :
		Matrix::ProjectionOrtho((float)camera.renderWidth, (float)camera.renderHeight, camera.nearZ, camera.farZ, 20);

	camera.cameraViewProjMatrixCB.contents->vproj = vMat * pMat;
	camera.cameraViewProjMatrixCB.contents->cameraPos = camera.position;

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(3, camera.cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());
}
