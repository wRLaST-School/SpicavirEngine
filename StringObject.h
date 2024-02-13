#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <Util.h>
#include <TextDrawer.h>

class StringObject :
    public IComponent
{
public:
    ComponentFactoryRegister(StringObject)

    void Draw() override;

    void OnInspectorWindowDraw() override;

	void WriteParamJson(nlohmann::json& jsonObj);

	void ReadParamJson(const nlohmann::json& paramsObject);
private:
    std::string str_;
    Float2 pos_ = { (float)Util::GetWinWidth() / 2, (float)Util::GetWinHeight() / 2 };
    Color color_ = Color::White;
    StringOptions opt_;
public:

    StringObject() = default;
};

