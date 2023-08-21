#include "stdafx.h"
#include "BTSequencerNode.h"

void BT::SequencerNode::OnStart()
{
}

BT::Status BT::SequencerNode::Update()
{
	if (children_.size() <= activeIndex)
	{
		InitNode();
		return Status::Success;
	}

	Status childStatus = children_.at(activeIndex)->Update();

	if (childStatus == Status::Completed || childStatus == Status::Success)
	{
		activeIndex++;
		if (activeIndex >= children_.size())
		{
			InitNode();
			return Status::Success;
		}

		return Status::Running;
	}

	if (childStatus == Status::Failure)
	{
		InitNode();
		return Status::Failure;
	}

	return Status::Running;
}

void BT::SequencerNode::OnEnd()
{
}

void BT::SequencerNode::OnAbort()
{
}

void BT::SequencerNode::SetParam(std::string param)
{
}

void BT::SequencerNode::InitNode()
{
	activeIndex = 0;
}
