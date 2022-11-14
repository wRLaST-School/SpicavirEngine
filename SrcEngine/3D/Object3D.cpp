#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>

void Object3D::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	mRot = Matrix::RotRollPitchYaw(rotation);

	mTrans = Matrix::Translation(position);

	matWorld = Matrix::Identity();
	matWorld *= mScale;
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
}

void Object3D::Draw()
{
	transformCB.contents->mat = matWorld;

	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	Light::Use();
	Camera::UseCurrent();

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(model->material.textureKey));

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(short), 1, 0, 0, 0);
}

void Object3D::Draw(TextureKey key)
{
	transformCB.contents->mat = matWorld;

	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	Light::Use();
	Camera::UseCurrent();

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(short), 1, 0, 0, 0);
}
