#include "Object3D.h"

void Object3D::UpdateMatrix()
{
	XMMATRIX mScale, mRot, mTrans;

	mScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	mRot = XMMatrixIdentity();
	mRot *= XMMatrixRotationZ(rotation.z);
	mRot *= XMMatrixRotationX(rotation.x);
	mRot *= XMMatrixRotationY(rotation.y);

	mTrans = XMMatrixTranslation(posision.x, posision.y, posision.z);

	matWorld = XMMatrixIdentity();
	matWorld *= mScale;
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
}

void Object3D::Draw(const XMMATRIX& viewProj)
{
	transformCB.contents->mat = matWorld * viewProj;

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(short), 1, 0, 0, 0);

}
