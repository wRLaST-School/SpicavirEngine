#include "stdafx.h"
#include "BTENode.h"
#include <BehaviorTree.h>

BTENode::BTENode()
{
	node_ = std::make_unique<BT::SequencerNode>("");
	node_->editorNodePtr = this;
}

void BTENode::ChangeNodeType(std::string type)
{
	std::unique_ptr<BT::INode> lastNode = std::move(node_);
	//NodeType‚É‚æ‚Á‚Ä•ªŠò
	if (type == "Action")
	{
		node_ = std::make_unique<BT::ActionNode>(lastNode->GetParam());
	}
	else if (type == "Loop")
	{
		node_ = std::make_unique<BT::LoopNode>(lastNode->GetParam());
	}
	else if (type == "Selector")
	{
		node_ = std::make_unique<BT::SelectorNode>(lastNode->GetParam());
	}
	else if (type == "Sequencer")
	{
		node_ = std::make_unique<BT::SequencerNode>(lastNode->GetParam());
	}
	else
	{
		node_ = std::make_unique<BT::SequencerNode>(lastNode->GetParam());
	}

	node_->editorNodePtr = this;
	node_->ChangeParent(lastNode->GetParent());
	node_->children_ = lastNode->children_;
	node_->parentBT_ = lastNode->parentBT_;
}
