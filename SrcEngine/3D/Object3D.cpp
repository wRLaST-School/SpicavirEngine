#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>
#include <SpMath.h>
#include <SpRenderer.h>

void Object3D::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	if (rotMode == RotMode::Quaternion)
	{
		mRot = rotation.GetRotMat();
	}
	else if (rotMode == RotMode::Euler)
	{
		mRot = Matrix::Identity();
		mRot *= Matrix::RotRollPitchYaw(rotationE);
	}

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
	/*GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	Light::Use();
	Camera::UseCurrent();*/

	SpRenderer::DrawCommand([&] {
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

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
	}, SpRenderer::Stage::Opaque);
}

void Object3D::Draw(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;

	//GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	//GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	//Light::Use();
	//Camera::UseCurrent();
	SpRenderer::DrawCommand([&, key] {
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		if(model->materialCBs.size())
			GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
		
		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Opaque);
}

void Object3D::DrawCommands(const TextureKey& key)
{
	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
	
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
}

void Object3D::DrawAdd()
{
	if (model->material.size())
	{
		DrawAdd(model->material.front().textureKey);
	}
	else
	{
		DrawAdd("notexture");
	}
}

void Object3D::DrawAdd(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());


		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetWDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetWDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Add);
}

void Object3D::DrawAlpha()
{
	if (model->material.size())
	{
		DrawAlpha(model->material.front().textureKey);
	}
	else
	{
		DrawAlpha("notexture");
	}
}

void Object3D::DrawAlpha(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	this->alphaTexKey = key;
	SpRenderer::RegisterAlphaObj(this);
}
