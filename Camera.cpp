#include "Camera.h"

Camera::Camera()
{
}

void Camera::SetRenderSize(UINT w, UINT h)
{
	renderWidth = w;
	renderHeight = h;
}

void Camera::Set(Camera& camera)
{
	D3D12_VIEWPORT viewport{};

	viewport.Width = camera.renderWidth;
	viewport.Height = camera.renderHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // �؂蔲�����W��
	scissorrect.right = scissorrect.left + camera.renderWidth;        // �؂蔲�����W�E
	scissorrect.top = 0;                                        // �؂蔲�����W��
	scissorrect.bottom = scissorrect.top + camera.renderHeight;       // �؂蔲�����W��

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	Matrix vMat = camera.targetMode == CameraTargetMode::LookAt ? 
		Matrix::ViewLookAt(camera.position, camera.target, camera.matWorld.ExtractAxisY()) : 
		Matrix::ViewLookTo(camera.position, camera.matWorld.ExtractAxisZ(), camera.matWorld.ExtractAxisY());

	Matrix pMat = camera.projectionMode == ProjectionMode::Perspective ? 
		Matrix::Projection(camera.fov, (float)camera.renderWidth / (float)camera.renderHeight, camera.nearZ, camera.farZ) :
		Matrix::ProjectionOrtho((float)camera.renderWidth, (float)camera.renderHeight, camera.nearZ, camera.farZ);

	camera.cameraViewProjMatrixCB.contents->vproj = vMat * pMat;
	camera.cameraViewProjMatrixCB.contents->cameraPos = camera.position;

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(3, camera.cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());
}
