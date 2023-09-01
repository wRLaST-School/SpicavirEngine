#include "stdafx.h"
#include "BTEditor.h"
#include <SpImGui.h>

void BTEditor::Draw()
{
    for (auto& o : GetInstance()->editorObjects)
    {
        o.Draw();
    }


    SpImGui::Command([&] {
        if (ImGui::Begin("Master"))
        {
            if (ImGui::Button("New Node"))
            {
                BTEditor* ins = GetInstance();
                ins->tree_.root->AddNode<BT::ActionNode>("");
                std::string uniqueName = "##";
                uniqueName += std::to_string(ins->id);
                ins->id++;
                ins->editorObjects.emplace_back(ins->tree_.root->LastPtr(), uniqueName, ins);
                ins->tree_.root->Last()->editorNodePtr = &ins->editorObjects.back();
            }
        }
        ImGui::End();
    });
}

BTENode* BTEditor::GetSelected()
{
    return selected_;
}

void BTEditor::ClearSelected()
{
    selected_ = nullptr;
}

void BTEditor::SetSelected(BTENode* node)
{
    selected_ = node;
}

BTEditor* BTEditor::GetInstance()
{
    static BTEditor ins;
    return &ins;
}

BTEditor::BTEditor()
{
    std::string uniqueName = "##";
    uniqueName += std::to_string(id++);

    editorObjects.emplace_back(&tree_.root, uniqueName, this);
}