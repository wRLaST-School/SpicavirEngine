#include "stdafx.h"
#include "BTEditor.h"

void BTEditor::Draw()
{
    for (auto& o : GetInstance()->editorObjects)
    {
        o.Draw();
    }
}

BTEditor* BTEditor::GetInstance()
{
    static BTEditor ins;
    return &ins;
}

BTEditor::BTEditor()
{
    std::string uniqueName = "Node##";
    uniqueName += std::to_string(id++);

    editorObjects.emplace_back(&tree_.root, uniqueName);
}