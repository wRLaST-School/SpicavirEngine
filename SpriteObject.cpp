#include "stdafx.h"
#include "SpriteObject.h"
#include <Sprite.h>
#include <SpImGui.h>
#include <Input.h>
#include <SpDS.h>

void SpriteObject::Update()
{
}

void SpriteObject::Draw()
{
	SpDS::DrawRotaGraph((int32_t)position.x, (int32_t)position.y,
		scale.x, scale.y,
		0.f,
		tex
	);
}

void SpriteObject::OnInspectorWindowDraw()
{
	ImGui::InputFloat2("Pos", &position.x);
	ImGui::InputFloat2("Scale", &scale.x);

	SpImGui::InputText("Texture", &tex, NULL);

	//gizmo
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
		ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGuizmo::AllowAxisFlip(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	ImGuizmo::Enable(true);

	if (Input::Key::Down(DIK_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (Input::Key::Down(DIK_S))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	Matrix view;
	Matrix proj = Sprite::sProj;

	Matrix matWorld = Matrix(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, 0, 1
	);

	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), mCurrentGizmoOperation, ImGuizmo::WORLD, &matWorld[0][0], NULL, NULL);

	if (ImGuizmo::IsUsing())
	{
		scale.x = matWorld[0][0];
		scale.y = matWorld[1][1];

		position.x = matWorld[3][0];
		position.y = matWorld[3][1];
	}
}

void SpriteObject::WriteParamJson(nlohmann::json& jsonObj)
{
	jsonObj["Texture"] = tex;
	jsonObj["posX"] = position.x;
	jsonObj["posY"] = position.y;
	jsonObj["scaleX"] = scale.x;
	jsonObj["scaleY"] = scale.y;
}

void SpriteObject::ReadParamJson(const nlohmann::json& jsonObj)
{
	tex = jsonObj["Texture"];
	position.x = jsonObj["posX"];
	position.y = jsonObj["posY"];
	scale.x = jsonObj["scaleX"];
	scale.y = jsonObj["scaleY"];
}