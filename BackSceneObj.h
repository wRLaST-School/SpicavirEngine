#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
class BackSceneObj :
    public IComponent
{
public:
    ComponentFactoryRegister(BackSceneObj)

    void Update() override;

    void Draw() override;
};

