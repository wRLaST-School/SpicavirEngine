#pragma once
#include "BTNodeBase.h"
namespace BT {
    class ConditionNode :
        public INode
    {
    public:
        void OnStart() override;
        BT::Status Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;

        void InitNode();
        std::string GetNodeType() override;

        int32_t selectedIndex = -1;
    };
}

