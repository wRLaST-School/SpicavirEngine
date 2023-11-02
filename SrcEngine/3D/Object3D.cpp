#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>
#include <SpMath.h>
#include <SpRenderer.h>
#include <SpImGui.h>
#include <Input.h>
#include <DockPanel.h>

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

void Object3D::DecomposeMatrix()
{
	Vec3 axisX = matWorld.ExtractAxisX();
	Vec3 axisY = matWorld.ExtractAxisY();
	Vec3 axisZ = matWorld.ExtractAxisZ();

	scale.x = axisX.GetLength();
	scale.y = axisY.GetLength();
	scale.z = axisZ.GetLength();

	position.x = matWorld[3][0];
	position.y = matWorld[3][1];
	position.z = matWorld[3][2];

	axisX.Norm(scale.x);
	axisY.Norm(scale.y);
	axisZ.Norm(scale.z);

	Matrix wMatNoScale(
		static_cast<Float4>(static_cast<Float3>(axisX)),
		static_cast<Float4>(static_cast<Float3>(axisY)),
		static_cast<Float4>(static_cast<Float3>(axisZ)),
		{ position.x, position.y, position.z, 1.f }
	);

	rotationE.x = atan2f(wMatNoScale[1][2], wMatNoScale[2][2]);
	rotationE.y = atan2f(-wMatNoScale[0][2], sqrtf(wMatNoScale[1][2] * wMatNoScale[1][2] + wMatNoScale[2][2] * wMatNoScale[2][2]));
	rotationE.z = atan2f(wMatNoScale[0][1], wMatNoScale[0][0]);
}

void Object3D::Draw()
{
	if (texture != "")
		Draw(texture);

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
	ImGui::InputFloat3("Translation", &position.x);

	if (rotMode == RotMode::Euler)
	{
		ImGui::DragFloat3("Rotation", &rotationE.x, PIf / 360.f);
	}
	else
	{
		ImGui::Text("vvv Gizmo Does Not Work Currently vvv");
		ImGui::InputFloat4("Rotation", &rotation.w);
	}

	ImGui::InputFloat3("Scale", &scale.x);

	ImGui::Checkbox("Use Quaternion Rotation", reinterpret_cast<bool*>(&rotMode));

	ImGui::ColorEdit4("Brightness", reinterpret_cast<float*>(brightnessCB.contents));

	ImGui::Separator();

	const size_t bufSize = 256;
	char buf[bufSize];
	strncpy_s(buf, texture.c_str(), std::min(bufSize, texture.length()));
	if (ImGui::InputText("Texture", buf, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		texture = buf;
	};

	UpdateMatrix();
}

void Object3D::DrawGizmo()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, 
		ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	ImGuizmo::Enable(true);

	if (Input::Key::Down(DIK_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (Input::Key::Down(DIK_R))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (Input::Key::Down(DIK_S)) 
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	Matrix view = Camera::sCurrent->GetViewMat();
	Matrix proj = Camera::sCurrent->GetProjMat();

	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), mCurrentGizmoOperation, mCurrentGizmoMode, &matWorld[0][0], NULL, NULL);

	if (ImGuizmo::IsUsing())
	{
		Float3 rot;
		matWorld.DecomposeTransform(&position, &rot, &scale);

		Vec3 deltaRot = static_cast<Vec3>(rot) - rotationE;

		rotationE += deltaRot;

		UpdateMatrix();
	}
}
