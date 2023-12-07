#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>

struct ServerSideData {
    Vec2 playerPos;
    int playerHealth;

    bool isReady = false;
};

struct ClientSideData {
    float powSiz = 32.f;
    int powNumBul = 4;
    float powSpd = 10.0f;
    int powGain = 50;

    bool isReady = false;

    struct BulletData {
        Vec2 path[600];
        bool active = false;
    } bullets[64];
};

class NetworkManager :
    public IComponent
{
public:
    ComponentFactoryRegister(NetworkManager);

    void Init() override;

    ServerSideData srvData;
    ClientSideData clnData;

private:
    void ClientThreadFunc();

    void ServerThreadFunc();
};

