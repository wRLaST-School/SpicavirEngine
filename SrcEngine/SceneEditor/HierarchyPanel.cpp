#include "stdafx.h"
#include "HierarchyPanel.h"
#include <SpImGui.h>
#include <IComponent.h>
#include <SceneManager.h>
#include <InspectorWindow.h>
#include <Object3D.h>
#include <Input.h>

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

        if (Input::Key::Triggered(DIK_DELETE))
        {
            if (!(InspectorWindow::GetSelected<IScene>()))
            {
                IComponent* cmp = InspectorWindow::GetSelected<IComponent>();
                if (cmp)
                {
                    cmp->PrepDelete();
                }
            }
        }
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

    DragDropTarget(current);
    
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

void HierarchyPanel::DragDropTarget(IComponent* current)
{
    if (ImGui::BeginDragDropTarget())
    {
        DDTargetTexture(current);
        DDTargetModel(current);

        ImGui::EndDragDropTarget();
    }
}

void HierarchyPanel::DDTargetTexture(IComponent* current)
{
    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RES_WINDOW_ITEM_TEXTURE");

    if (payload) {
        const char* texKey = reinterpret_cast<const char*>(payload->Data);

        Object3D* obj = dynamic_cast<Object3D*>(current);

        if (obj)
        {
            obj->texture = texKey;
        }
    }
}

void HierarchyPanel::DDTargetModel(IComponent* current)
{
    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RES_WINDOW_ITEM_MODEL");

    if (payload) {
        const char* modelKey = reinterpret_cast<const char*>(payload->Data);

        Object3D* obj = dynamic_cast<Object3D*>(current);

        if (obj)
        {
            obj->model = ModelManager::GetModel(modelKey);
        }
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
