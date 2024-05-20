#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <Util.h>
#include <SpTextureManager.h>

class DockPanel;

class SpriteObject :
    public IComponent
{
public:
    friend DockPanel;

    ComponentFactoryRegister(SpriteObject)

    void Update() override;

    void Draw() override;

    void OnInspectorWindowDraw() override;

    void WriteParamJson(nlohmann::json& jsonObj) override;

    void ReadParamJson(const nlohmann::json& jsonObject) override;

private:
    Float2 position = { (float)Util::GetWinWidth() / 2, (float)Util::GetWinHeight() / 2 };
    Float3 scale = { 1.f, 1.f };
    TextureKey tex;
};
