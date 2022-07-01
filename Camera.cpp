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
	Matrix vMat = camera.targetMode == CameraTargetMode::LookAt ? 
		Matrix::ViewLookAt(camera.posision, camera.target, camera.matWorld.ExtractAxisY()) : 
		Matrix::ViewLookTo(camera.posision, camera.matWorld.ExtractAxisZ(), camera.matWorld.ExtractAxisY());

	Matrix pMat = Matrix::Projection(camera.fov, (float)camera.renderWidth / (float)camera.renderHeight, camera.nearZ, camera.farZ);

	camera.cameraViewProjMatrixCB.contents->vproj = vMat * pMat;

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(3, camera.cameraViewProjMatrixCB.buffer->GetGPUVirtualAddress());
}
