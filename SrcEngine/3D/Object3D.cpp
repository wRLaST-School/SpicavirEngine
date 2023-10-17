#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>
#include <SpMath.h>
#include <SpRenderer.h>
#include <SpImGui.h>

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

	SpRenderer::DrawCommand([&] {
		if (model->material.size())
		{
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(model->material.front().textureKey));
		}
		else
		{
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle("notexture"));
		}

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.back().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
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
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		if(model->materialCBs.size())
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
		
		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Opaque);
}

void Object3D::DrawCommands(const TextureKey& key)
{
	GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
	
	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(5, model->bMatrixCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, miscCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(7, SpTextureManager::GetGPUDescHandle("dissolveMap"));

	GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
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
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());
		
		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(5, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(7, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
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

void Object3D::DrawParams()
{
	ImGui::DragFloat3("Position", &position.x, 0.1f);
	ImGui::DragFloat3("Scale", &scale.x, 0.05f);
	if (rotMode == RotMode::Euler)
	{
		ImGui::DragFloat3("Rotation", &rotationE.x, PIf / 360.f);
	}
	else
	{
		ImGui::InputFloat4("Rotation", &rotation.w);
	}
	ImGui::Checkbox("Use Quaternion Rotation", reinterpret_cast<bool*>(&rotMode));

	ImGui::ColorEdit4("Brightness", reinterpret_cast<float*>(brightnessCB.contents));
}
