#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>
#include <SpMath.h>

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
	ID3D12Resource* res = nullptr;

	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	Light::Use();
	Camera::UseCurrent();

	if (model->material.size())
	{
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(model->material.front().textureKey));
	}
	else
	{
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle("notexture"));
	}

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.back().buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(unsigned int), 1, 0, 0, 0);
}

void Object3D::Draw(TextureKey key)
{
	transformCB.contents->mat = matWorld;

	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	Light::Use();
	Camera::UseCurrent();

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(unsigned int), 1, 0, 0, 0);
}
