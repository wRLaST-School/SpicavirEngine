#pragma once
#include "BTNodeBase.h"
namespace BT {
    class ActionNode :
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

    private:
        std::string funcKey_;
    };
}

