#include "stdafx.h"
#include "HierarchyPanel.h"
#include <SpImGui.h>
#include <IComponent.h>
#include <SceneManager.h>
#include <InspectorWindow.h>

void HierarchyPanel::Draw()
{
    SpImGui::Command(std::bind(&HierarchyPanel::OnImGuiRender, this));
}

void HierarchyPanel::OnImGuiRender()
{
    itemIndex = 0;
    if (ImGui::Begin("Hierarchy"))
    {
        ShowItemRecursive(SceneManager::currentScene.get());
    }

    ImGui::End();
}

void HierarchyPanel::ShowItemRecursive(IComponent* current)
{
    std::string taggedName = current->name_ + std::string("##") + std::to_string(itemIndex);
    itemIndex++;

    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;

    nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
    nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
    
    if (current->components_.size() == 0)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool treeNodeTriggered = ImGui::TreeNodeEx(taggedName.c_str(), nodeFlags);
    
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        InspectorWindow::SelectObject(current);
    }

    if (treeNodeTriggered)
    {
        for (auto& child : current->components_)
        {
            ShowItemRecursive(child.second.get());
        }

        ImGui::TreePop();
    }
}

void HierarchyPanel::SDraw()
{
    GetInstance()->Draw();
}

HierarchyPanel* HierarchyPanel::GetInstance()
{
    static HierarchyPanel ins;
    return &ins;
}
