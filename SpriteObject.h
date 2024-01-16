#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <Util.h>
#include <SpTextureManager.h>
class SpriteObject :
    public IComponent
{
public:
    ComponentFactoryRegister(SpriteObject)

    void Update() override;

    void Draw() override;

    void DrawParams() override;

    void WriteParamJson(nlohmann::json& jsonObj) override;

    void ReadParamJson(const nlohmann::json& jsonObject) override;

private:
    Float2 position = { (float)Util::GetWinWidth() / 2, (float)Util::GetWinHeight() / 2 };
    Float3 scale = {1.f, 1.f};
    TextureKey tex;
};

