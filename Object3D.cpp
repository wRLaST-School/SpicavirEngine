#include "Object3D.h"

void Object3D::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	mRot = Matrix::RotRollPitchYaw(rotation);

	mTrans = Matrix::Translation(posision);

	matWorld = Matrix::Identity();
	matWorld *= mScale;
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
}

void Object3D::Draw(const Matrix& viewProj)
{
	transformCB.contents->mat = matWorld * viewProj;

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(short), 1, 0, 0, 0);
}
