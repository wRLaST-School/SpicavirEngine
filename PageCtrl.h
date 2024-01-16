#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
class PageCtrl :
    public IComponent
{
public:
    ComponentFactoryRegister(PageCtrl)

    void Update() override;

    void DrawParams() override;

private:
    inline static std::vector<std::string> files = {
        "Assets/Scene/page1.scene",
        "Assets/Scene/page2.scene",
        "Assets/Scene/page3.scene",
        "Assets/Scene/page4.scene",
        "Assets/Scene/page5.scene",
        "Assets/Scene/page6.scene",
        "Assets/Scene/page7.scene",
        "Assets/Scene/page8.scene",
        "Assets/Scene/page9.scene",
        "Assets/Scene/page10.scene",
    };
    inline static int page = 0;
};

