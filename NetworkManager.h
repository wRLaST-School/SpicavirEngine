#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>

struct ServerSideData {
    Vec2 playerPos;
    int playerHealth;

    bool isReady = false;

    bool terminate = false;
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

    bool terminate = false;
};

class NetworkManager :
    public IComponent
{
public:
    ComponentFactoryRegister(NetworkManager);

    void Init() override;
    ~NetworkManager() {
        srvData.terminate = true;
        clnData.terminate = true;
    }

    ServerSideData srvData;
    ClientSideData clnData;

    inline static std::thread th;

private:
    void ClientThreadFunc();

    void ServerThreadFunc();
};

