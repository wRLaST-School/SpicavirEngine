#include "PointLight.h"
#include <SpImGui.h>
#include <LineDrawer.h>
#include <Light.h>
#include <Camera.h>

PointLight::PointLight():pos({ 0,0,0 }), color({ 0,0,0 }), att({ 0,0,0 }) {
	isActive = false;
	Light::RegisterPointLight(this);
}

PointLight::PointLight(const Float3& pos, const Float3& color, const Float3& att) :pos(pos), color(color), att(att), isActive(true) {
	Light::RegisterPointLight(this);
}

PointLight::~PointLight()
{
	Light::ClearPointLight(this);
}

void PointLight::DrawFrame()
{
	if(showFrame_)
		LineDrawer::DrawCube(pos, { .25f, .25f, .25f }, color);
}

void PointLight::OnInspectorWindowDraw()
{
	ImGui::SliderFloat3("Position", &pos.x, -10.f, 10.f);
	ImGui::SliderFloat3("Color", &color.x, 0.f, 3.f);
	ImGui::SliderFloat3("Attenuation", &att.x, 0.f, .2f);
	ImGui::Checkbox("Active", &isActive);
	ImGui::Checkbox("ShowFrame", &showFrame_);
}

void PointLight::DrawGizmo()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
		ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGuizmo::AllowAxisFlip(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	Matrix matWorld;

	matWorld[3][0] = pos.x;
	matWorld[3][1] = pos.y;
	matWorld[3][2] = pos.z;

	ImGuizmo::Enable(true);

	Matrix view = Camera::sCurrent->GetViewMat();
	Matrix proj = Camera::sCurrent->GetProjMat();

	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, &matWorld[0][0], NULL, NULL);

	if (ImGuizmo::IsUsing())
	{
		pos.x = matWorld[3][0];
		pos.y = matWorld[3][1];
		pos.z = matWorld[3][2];
	}
}
