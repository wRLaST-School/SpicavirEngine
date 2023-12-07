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

    void UpgradeSize();
    void UpgradeSpd();
    void UpgradeNum();
    void UpgradeGain();

    void DrawParams() override;

    void Activate();

    void ReceiveClientBulletData();

private:
    static const size_t MAX_LINE_TIME = 600;

    size_t lineTime = 0;

    Vec2 line[MAX_LINE_TIME];

    int curBulletNum = 0;

    int coin = 50;

    float powSiz = 32.f;
    int powNumBul = 4;
    float powSpd = 10.0f;
    int powGain = 50;

    int costSiz = 50;
    int costSpd = 50;
    int costNum = 100;
    int costGain = 100;
};

