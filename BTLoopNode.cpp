#include "stdafx.h"
#include "BTLoopNode.h"

void BT::LoopNode::OnStart()
{
}

BT::Status BT::LoopNode::Update()
{
    if (loopCount_ <= param_ || param_ == 0)
    {
        BT::Status status = children_.front()->Update();

        if (status == Status::Completed || status == Status::Success || status == Status::Failure)
        {
            loopCount_++;
            if (loopCount_ >= param_ && param_ != 0)
            {
                return Status::Completed;
            }
        }
    }

    return Status::Running;
}

void BT::LoopNode::OnEnd()
{
}

void BT::LoopNode::OnAbort()
{
}

void BT::LoopNode::SetParam(std::string param)
{
    param_ = std::stoi(param);
}

void BT::LoopNode::InitNode()
{
    loopCount_ = 0;
}
