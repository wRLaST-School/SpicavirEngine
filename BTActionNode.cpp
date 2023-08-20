#include "stdafx.h"
#include "BTActionNode.h"
#include "BehaviorTree.h"

void BT::ActionNode::OnStart()
{
}

BT::Status BT::ActionNode::Update()
{
	return parentBT_->GetFactory()->GetActionFunc(funcKey_)();
}

void BT::ActionNode::OnEnd()
{
}

void BT::ActionNode::OnAbort()
{
}

void BT::ActionNode::SetParam(std::string param)
{
	funcKey_ = param;
}

void BT::ActionNode::InitNode()
{
}
