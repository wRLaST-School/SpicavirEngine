#include "stdafx.h"
#include "BehaviorTree.h"

BT::BehaviorTree::BehaviorTree()
{
	root = std::make_unique<INode>();
}

void BT::BehaviorTree::SetFactory(const BehaviorTreeFactory& factory)
{
	funcFactory_ = factory;
}

void BT::BehaviorTree::Tick()
{
	root->Update();
}
