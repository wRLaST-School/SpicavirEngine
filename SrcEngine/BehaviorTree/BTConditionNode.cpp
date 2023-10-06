#include "stdafx.h"
#include "BTConditionNode.h"
#include <BehaviorTree.h>

void BT::ConditionNode::OnStart()
{
}

BT::Status BT::ConditionNode::Update()
{
	bool doesConditionMatch = parentBT_->GetFactory()->GetConditionFunc(param_)();
	if (selectedIndex < 0)
	{
		if (doesConditionMatch)
		{
			selectedIndex = 0;
		}
		else if (children_.size() >= 2)
		{
			selectedIndex = 1;
		}
	}

	Status childStatus = Status::Error;
	int32_t ind = 0;
	for (auto itr = children_.begin(); itr != children_.end(); itr++)
	{
		if (ind == selectedIndex)
		{
			childStatus = (*itr)->Update();
		}
		ind++;
	}

	if (childStatus == Status::Completed || childStatus == Status::Success)
	{
		InitNode();
		return Status::Success;
	}

	if (childStatus == Status::Failure)
	{
		InitNode();
		return Status::Failure;
	}

	return Status::Running;
}

void BT::ConditionNode::OnEnd()
{
}

void BT::ConditionNode::OnAbort()
{
}

void BT::ConditionNode::SetParam(std::string param)
{
	INode::SetParam(param);
}

void BT::ConditionNode::InitNode()
{
	selectedIndex = -1;
}

std::string BT::ConditionNode::GetNodeType()
{
	return "Condition";
}
