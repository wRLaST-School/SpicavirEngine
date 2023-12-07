#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
class ClientPlayer :
    public IComponent
{
public:
    ComponentFactoryRegister(ClientPlayer)

    void Init() override;

    void Update() override;

    void Draw() override;
};

