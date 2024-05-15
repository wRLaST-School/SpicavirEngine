#include "IScriptObject.h"
#include "IScriptObject.h"

ScriptComponent* IScriptObject::This()
{
    return body;
}

std::string IScriptObject::GetClassString(void)
{
    return "IScriptObject";
}
