#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <Circle.h>
class ServerPlayer :
    public IComponent
{
public:
    ComponentFactoryRegister(ServerPlayer)

    void Init() override;
    void Update() override;
    void Draw() override;
    void Damage();

private:
    int hp = 5;
    float spd = 8.f;
};

