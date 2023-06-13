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
		Matrix::ProjectionOrtho((int32_t)current->renderWidth, (int32_t)current->renderHeight, current->nearZ, current->farZ, 20);

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

Float3 Camera::GetWorldPosFromScreen(const Float2& screen, float depth)
{
	if (useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		renderWidth = (float)wnd->width;
		renderHeight = (float)wnd->height;
	}

	Matrix vMat = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());

	Matrix pMat = projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(fov, (float)renderWidth / (float)renderHeight, nearZ, farZ) :
		Matrix::ProjectionOrtho((int32_t)renderWidth, (int32_t)renderHeight, nearZ, farZ, 20);

	Matrix vport(
		(float)GetSpWindow()->width / 2, 0.f, 0.f, 0.f,
		0.f, (float)-GetSpWindow()->height / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(float)GetSpWindow()->width / 2, (float)GetSpWindow()->height / 2, 0, 1
	);

	/*depth /= farZ - nearZ;*/

	Matrix wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 0.f;

	Matrix vpvpMat = vMat * pMat * vport;
	Matrix invMat = -vpvpMat;

	Matrix result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 nearPos(result[3][0], result[3][1], result[3][2]);

	wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 1.f;

	result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 farPos(result[3][0], result[3][1], result[3][2]);

	Vec3 ray = farPos - nearPos;

	return nearPos + ray.SetLength(depth);
}

Ray Camera::GetScreenPosRay(const Float2& screen)
{
	if (useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		renderWidth = (float)wnd->width;
		renderHeight = (float)wnd->height;
	}

	Matrix vMat = targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(position, target, matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(position, matWorld.ExtractAxisZ(), matWorld.ExtractAxisY());

	Matrix pMat = projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(fov, (float)renderWidth / (float)renderHeight, nearZ, farZ) :
		Matrix::ProjectionOrtho((int32_t)renderWidth, (int32_t)renderHeight, nearZ, farZ, 20);

	Matrix vport(
		(float)GetSpWindow()->width / 2, 0.f, 0.f, 0.f,
		0.f, -(float)GetSpWindow()->height / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(float)GetSpWindow()->width / 2, (float)GetSpWindow()->height / 2, 0, 1
	);

	/*depth /= farZ - nearZ;*/

	Matrix wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 0.f;

	Matrix vpvpMat = vMat * pMat * vport;
	Matrix invMat = -vpvpMat;

	Matrix result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 nearPos(result[3][0], result[3][1], result[3][2]);

	wMat = Matrix::Identity();
	wMat[3][0] = screen.x;
	wMat[3][1] = screen.y;
	wMat[3][2] = 1.f;

	result = wMat * (-vpvpMat);

	result[3][0] /= result[3][3];
	result[3][1] /= result[3][3];
	result[3][2] /= result[3][3];

	Vec3 farPos(result[3][0], result[3][1], result[3][2]);

	Vec3 ray = farPos - nearPos;

	Ray r;
	r.ray = ray;
	r.origin = nearPos;
	return r;
}

Camera* Camera::current = nullptr;
