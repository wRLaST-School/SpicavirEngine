#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>

struct ServerSideData {

};

struct ClientSideData {

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

