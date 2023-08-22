#include "stdafx.h"
#include "BTRootNode.h"

void BT::RootNode::OnStart()
{
}

BT::Status BT::RootNode::Update()
{
	if (children_.size())
	{
		return children_.front()->Update();
	}

	return Status::Success;
}

void BT::RootNode::OnEnd()
{
}

void BT::RootNode::OnAbort()
{
	INode::OnAbort();
}

void BT::RootNode::SetRootBT(BehaviorTree* bt)
{
	parentBT_ = bt;
}

void BT::RootNode::SetParam(std::string param)
{
	INode::SetParam(param);
}

std::string BT::RootNode::GetNodeType()
{
	return "Root";
}
