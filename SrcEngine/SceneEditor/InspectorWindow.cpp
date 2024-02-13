#include "stdafx.h"
#include "InspectorWindow.h"
#include <SpImGui.h>
#include <ComponentFactory.h>

void InspectorWindow::SelectObject(IComponent* component)
{
	GetInstance()->selected_ = component;
}

void InspectorWindow::DrawWindow()
{
	SpImGui::Command([&] {
		static bool open = true;

		ImGui::Begin("Inspector Window", &open, ImGuiWindowFlags_MenuBar);
		if (selected_)
		{
			selected_->OnInspectorWindowDraw();

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("SelectComponentPopup");
			}

			//コンポーネント選択ポップアップの設定
			if (ImGui::BeginPopup("SelectComponentPopup"))
			{
				for (auto& item : ComponentFactory::GetInstance()->factory)
				{
					if (ImGui::MenuItem(item.first.c_str()))
					{
						ComponentFactory::AddChildComponent(selected_, item.first.c_str(), item.first.c_str());
					}
				}
				ImGui::EndPopup();
			}
		}

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
