#include "stdafx.h"
#include "BTSelectorNode.h"
#include <Util.h>

void BT::SelectorNode::OnStart()
{
}

BT::Status BT::SelectorNode::Update()
{
	if (selectedIndex < 0)
	{
		selectedIndex = Util::RNG(0, children_.size() - 1, true);
	}

	Status childStatus = children_.at(selectedIndex)->Update();

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

	InitNode();
	return Status::Running;
}

void BT::SelectorNode::OnEnd()
{
}

void BT::SelectorNode::OnAbort()
{
}

void BT::SelectorNode::SetParam(std::string param)
{
}

void BT::SelectorNode::InitNode()
{
	selectedIndex = -1;
}
