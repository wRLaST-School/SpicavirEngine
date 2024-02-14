#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <DLLObject.h>
class ScriptComponent :
    public IComponent
{
public:
    ComponentFactoryRegister(ScriptComponent)

    void Init() override;
    void Update() override;
    void Draw() override;

    void OnInspectorWindowDraw() override;

    void WriteParamJson(nlohmann::json& jsonObj) override;
    void ReadParamJson(const nlohmann::json& jsonObj) override;

private:
    void CompileScript();
    void LoadDLL();

    std::string filePath;

    Libra::DLLObject dllobj_;

    inline static std::string compileFolder = "Resources/Compiled/";
};

