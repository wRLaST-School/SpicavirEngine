#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
class TestMoveObject :
    public IComponent
{
public:
    ComponentFactoryRegister(TestMoveObject)
    
    void Update()override;

    void Draw()override;

    void OnInspectorWindowDraw() override;

private:
    float spd = 0.2f;
};
