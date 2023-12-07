#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>

class Bullet :
    public IComponent
{
public:
    ComponentFactoryRegister(Bullet)

    Bullet();

    void Init() override;
    void Update() override;
    void Draw() override;

    void Activate();

    static const size_t MAX_LINE_TIME = 600;
    Vec2 path[MAX_LINE_TIME] = {};

    float speed = 8.f;
    float size = 32.f;

private:
    size_t timer = 0;

    size_t pathIndex = 0;

    Float2 pos_ = {-1000.f, -1000.f};

    float lastLength = 0.f;

    bool active = false;
};

