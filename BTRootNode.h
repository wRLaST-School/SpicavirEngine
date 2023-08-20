#pragma once
#include "BTNodeBase.h"

namespace BT {
    class RootNode :
        public INode
    {
    public:
        void OnStart() override;
        BT::Status Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;
    };
}

