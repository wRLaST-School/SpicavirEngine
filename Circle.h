#pragma once
#include <Color.h>
#include <ComponentFactory.h>
class CircleCollider : public IComponent
{
public:
    CircleCollider();

    ComponentFactoryRegister(CircleCollider)

    bool Collide(const CircleCollider& o);

    void Draw();
    void Update();
    void Init(Vec2 pos, float r);
    void Init();

    Vec2 pos_;
    float r_;
    Color color_ = Color::White;
};