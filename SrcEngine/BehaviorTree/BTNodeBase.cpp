#include "stdafx.h"
#include "BTNodeBase.h"

void BT::INode::OnAbort()
{
	OnEnd();
}

void BT::INode::SetParam(std::string param)
{
	param_ = param;
}

std::string BT::INode::GetParam()
{
	return param_;
}

const std::vector<std::unique_ptr<BT::INode>>& BT::INode::GetChildren()
{
	return children_;
}

BT::INode* BT::INode::Last()
{
	return children_.back().get();
}
