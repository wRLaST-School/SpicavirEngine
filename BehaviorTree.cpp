#include "stdafx.h"
#include "BehaviorTree.h"
#include <BTRootNode.h>

BT::BehaviorTree::BehaviorTree()
{
	root = std::make_unique<RootNode>();
	dynamic_cast<RootNode*>(root.get())->SetRootBT(this);
}

void BT::BehaviorTree::SetFactory(const BehaviorTreeFactory& factory)
{
	funcFactory_ = factory;
}

BT::BehaviorTreeFactory* BT::BehaviorTree::GetFactory()
{
	return &funcFactory_;
}

void BT::BehaviorTree::Tick()
{
	root->Update();
}
