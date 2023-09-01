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

	Status childStatus = Status::Error;;
	uint32_t ind = 0;
	for (auto itr = children_.begin(); itr != children_.end(); itr++)
	{
		if (ind == activeIndex)
		{
			childStatus = (*itr)->Update();
		}
		ind++;
	}

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
	INode::SetParam(param);
}

void BT::SequencerNode::InitNode()
{
	activeIndex = 0;
}

std::string BT::SequencerNode::GetNodeType()
{
	return "Sequencer";
}
