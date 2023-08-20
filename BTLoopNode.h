#pragma once
#include "BTNodeBase.h"
namespace BT {
    class LoopNode :
        public INode
    {
    public:
        void OnStart() override;
        BT::Status Update() override;
        void OnEnd() override;
        void OnAbort() override;

        void SetParam(std::string param) override;

        void InitNode();

    private:
        int32_t loopCount_ = 0;
        int32_t param_ = 0;
    };
}

