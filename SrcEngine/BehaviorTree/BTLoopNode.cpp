#include "stdafx.h"
#include "BTLoopNode.h"

void BT::LoopNode::OnStart()
{
}

BT::Status BT::LoopNode::Update()
{
    if (loopCount_ < param_ || param_ == 0)
    {
        BT::Status status = children_.front()->Update();

        if (status == Status::Completed || status == Status::Success || status == Status::Failure)
        {
            loopCount_++;
            if (loopCount_ >= param_ && param_ != 0)
            {
                InitNode();
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
    INode::SetParam(param);
    try {
        param_ = std::stoi(param);
    }
    catch (std::invalid_argument e)
    {
        param_ = 0;
    }
    catch (std::out_of_range e)
    {
        param_ = 0;
    }
}

void BT::LoopNode::InitNode()
{
    loopCount_ = 0;
}

std::string BT::LoopNode::GetNodeType()
{
    return "Loop";
}
