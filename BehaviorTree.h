#pragma once
#include <BehaviorTreeFactory.h>
#include <BTNodeBase.h>
#include <BTActionNode.h>
#include <BTLoopNode.h>
#include <BTRootNode.h>
#include <BTSelectorNode.h>
#include <BTSequencerNode.h>

namespace BT {
	class BehaviorTree
	{
	public:
		BehaviorTree();

		void SetFactory(const BehaviorTreeFactory& factory);
		BehaviorTreeFactory* GetFactory();

		void Tick();

		std::unique_ptr<INode> root;

	private:
		BehaviorTreeFactory funcFactory_;
	};
}

