#include "Camera.h"

#include <SpDirectX.h>
#include <SpEffekseer.h>

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
	rotation = Quaternion::Identity();

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
	sCurrent = &camera;
}

void Camera::UseCurrent()
{
	if (sCurrent->useWindowSize)
	{
		SpWindow* wnd = GetSpWindow();
		sCurrent->renderWidth = (float)wnd->width;
		sCurrent->renderHeight = (float)wnd->height;
	}

	D3D12_VIEWPORT viewport{};

	viewport.Width = sCurrent->renderWidth;
	viewport.Height = sCurrent->renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetSpDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + (LONG)sCurrent->renderWidth;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + (LONG)sCurrent->renderHeight;       // 切り抜き座標下

	GetSpDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	sCurrent->view = sCurrent->targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(sCurrent->position, sCurrent->target, sCurrent->matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(sCurrent->position, sCurrent->matWorld.ExtractAxisZ(), sCurrent->matWorld.ExtractAxisY());

	sCurrent->proj = sCurrent->projectionMode == ProjectionMode::Perspective ?
		Matrix::Projection(sCurrent->fov, (float)sCurrent->renderWidth / (float)sCurrent->renderHeight, sCurrent->nearZ, sCurrent->farZ) :
		Matrix::ProjectionOrtho((int32_t)sCurrent->renderWidth, (int32_t)sCurrent->renderHeight, sCurrent->nearZ, sCurrent->farZ, 20);

	sCurrent->cameraViewProjMatrixCB.contents->vproj = sCurrent->view * sCurrent->proj;
	sCurrent->cameraViewProjMatrixCB.contents->cameraPos = sCurrent->position;

	sCurrent->cameraViewProjMatrixCB.contents->billboardMat = GetCurrentCameraBillboardMat();

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(3, sCurrent->cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());

	//Effekseerの情報を更新
	std::function<Effekseer::Matrix44(Matrix)> SpMatToEfkMat = [](Matrix in) {
		Effekseer::Matrix44 out;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.Values[i][j] = in[i][j];
			}
		}

		return out;
	};

	Effekseer::Matrix44 efkViewMat = SpMatToEfkMat(sCurrent->view);
	Effekseer::Matrix44 efkProjMat = SpMatToEfkMat(sCurrent->proj);

	SpEffekseer::SetMatrices(efkViewMat, efkProjMat);
}

Matrix Camera::GetCurrentCameraBillboardMat()
{
	Matrix camRot = sCurrent->targetMode == CameraTargetMode::LookAt ?
		Matrix::ViewLookAt(sCurrent->position, sCurrent->target, sCurrent->matWorld.ExtractAxisY()) :
		Matrix::ViewLookTo(sCurrent->position, sCurrent->matWorld.ExtractAxisZ(), sCurrent->matWorld.ExtractAxisY());
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

Matrix Camera::GetViewMat()
{
	return view;
}

Matrix Camera::GetProjMat()
{
	return proj;
}

Camera* Camera::sCurrent = nullptr;
