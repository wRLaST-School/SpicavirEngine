#pragma once
#include <Color.h>
#include <ComponentFactory.h>
class Circle : public IComponent
{
public:
    Circle();

    ComponentFactoryRegister(Circle)

    bool Collide(const Circle& o);

    void Draw();
    void Update();
    void Init(Vec2 pos, float r);

    Vec2 pos_;
    float r_;
    Color color_ = Color::White;
};