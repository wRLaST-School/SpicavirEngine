#include "stdafx.h"
#include "BTEditor.h"

BTEditor* BTEditor::GetInstance()
{
    static BTEditor ins;
    return &ins;
}
