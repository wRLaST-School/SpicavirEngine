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

        void SetRootBT(BehaviorTree* bt);

        void SetParam(std::string param) override;
        std::string GetNodeType() override;
    };
}

