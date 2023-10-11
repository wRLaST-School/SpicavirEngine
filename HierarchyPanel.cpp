#include "stdafx.h"
#include "HierarchyPanel.h"
#include <SpImGui.h>
#include <IComponent.h>
#include <SceneManager.h>

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
    std::string taggedName = current->name + std::string("##") + std::to_string(itemIndex);
    itemIndex++;

    ImGuiTreeNodeFlags nodeFlags = 0;
    
    if (current->components_.size() == 0)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (ImGui::TreeNodeEx(taggedName.c_str(), nodeFlags))
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
