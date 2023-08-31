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

BT::INode* BT::INode::GetParent()
{
	return parent_;
}

const std::vector<std::unique_ptr<BT::INode>>& BT::INode::GetChildren()
{
	return children_;
}

BT::INode* BT::INode::Last()
{
	return children_.back().get();
}

void BT::INode::ChangeParent(INode* newParent)
{
	for (auto itr = parent_->children_.begin(); itr != parent_->children_.end(); itr++)
	{
		if (itr->get() == this)
		{
			itr = parent_->children_.erase(itr);
			break;
		}
	}

	newParent->children_.emplace_back(this);
	this->parent_ = newParent;
}
