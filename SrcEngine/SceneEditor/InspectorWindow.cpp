#include "stdafx.h"
#include "InspectorWindow.h"
#include <SpImGui.h>

void InspectorWindow::SelectObject(IComponent* component)
{
	GetInstance()->selected_ = component;
}

void InspectorWindow::DrawWindow()
{
	SpImGui::Command([&] {
		static bool open = true;
		ImGui::Begin("Inspector Window", &open, ImGuiWindowFlags_MenuBar);
		if (selected_) selected_->DrawParams();
		ImGui::End();
	});
}

void InspectorWindow::SDraw()
{
	GetInstance()->DrawWindow();
}

InspectorWindow* InspectorWindow::GetInstance()
{
	static InspectorWindow ins;
	return &ins;
}
