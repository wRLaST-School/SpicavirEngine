#include "stdafx.h"
#include "BTNodeBase.h"
#include <BehaviorTree.h>

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

const std::list<std::unique_ptr<BT::INode>>& BT::INode::GetChildren()
{
	return children_;
}

BT::INode* BT::INode::Last()
{
	return children_.back().get();
}

std::unique_ptr<BT::INode>* BT::INode::LastPtr()
{
	return &children_.back();
}

std::unique_ptr<BT::INode>* BT::INode::ChangeParent(INode* newParent)
{
	//—¼•û‚ ‚éê‡
	if (parent_ != nullptr && newParent != nullptr)
	{
		for (auto itr = parent_->children_.begin(); itr != parent_->children_.end(); itr++)
		{
			if (itr->get() == this)
			{
				newParent->children_.emplace_back();
				newParent->children_.back().swap(*itr);
				itr = parent_->children_.erase(itr);
				this->parent_ = newParent;
				return &newParent->children_.back();
			}
		}
	}
	//parent‚ª–³‚­‚Änew parent‚ª‚ ‚éê‡
	else if(parent_ == nullptr && newParent != nullptr)
	{
		if (parentBT_->root.get() == this)
		{
			return &parentBT_->root;
		}

		for (auto itr = parentBT_->floatingNodes.begin(); itr != parentBT_->floatingNodes.end(); itr++)
		{
			if (itr->get() == this)
			{
				newParent->children_.emplace_back();
				newParent->children_.back().swap(*itr);
				itr = parentBT_->floatingNodes.erase(itr);
				this->parent_ = newParent;
				return &newParent->children_.back();
			}
		}
	}
	//parent‚ª‚ ‚Á‚Änew parent‚ª–³‚¢ê‡
	else if (parent_ != nullptr && newParent == nullptr)
	{
		for (auto itr = parent_->children_.begin(); itr != parent_->children_.end(); itr++)
		{
			if (itr->get() == this)
			{
				parentBT_->floatingNodes.emplace_back();
				parentBT_->floatingNodes.back().swap(*itr);
				itr = parent_->children_.erase(itr);
				this->parent_ = nullptr;
				return &parentBT_->floatingNodes.back();
			}
		}
	}

	else //parent‚ànew parent‚à‚È‚¢ê‡
	{
		if (parentBT_->root.get() == this)
		{
			return &parentBT_->root;
		}

		for (auto itr = parentBT_->floatingNodes.begin(); itr != parentBT_->floatingNodes.end(); itr++)
		{
			if (itr->get() == this)
			{
				return &(*itr);
			}
		}
	}

	return nullptr;
}
