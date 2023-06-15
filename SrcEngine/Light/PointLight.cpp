#include "PointLight.h"
#include <SpImGui.h>
#include <LineDrawer.h>

void PointLight::DrawFrame()
{
	if(showFrame_)
		LineDrawer::DrawCube(pos, { .25f, .25f, .25f }, color);
}

void PointLight::DrawLightEditor()
{
	SpImGui::Command([&]() {
		ImGui::SetNextWindowPos(ImVec2(1500, 160));
		ImGui::SetNextWindowSize(ImVec2(300, 600));
		if (ImGui::Begin("Light Editor"))
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
		ImGui::End();
	});
}
