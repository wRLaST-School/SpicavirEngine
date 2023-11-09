#include "PointLight.h"
#include <SpImGui.h>
#include <LineDrawer.h>
#include <Light.h>

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

void PointLight::DrawParams()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::SliderFloat3("Position", &pos.x, -10.f, 10.f);
		ImGui::SliderFloat3("Color", &color.x, 0.f, 3.f);
		ImGui::SliderFloat3("Attenuation", &att.x, 0.f, .2f);
		ImGui::Checkbox("Active", &isActive);
		ImGui::Checkbox("ShowFrame", &showFrame_);

		ImGui::TreePop();
	}
}
