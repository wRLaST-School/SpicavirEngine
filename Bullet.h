#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>

class Bullet :
    public IComponent
{
public:
    ComponentFactoryRegister(Bullet)

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    size_t timer = 0;
};

