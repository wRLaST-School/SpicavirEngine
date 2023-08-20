#pragma once
#include <BehaviorTreeFactory.h>
#include <BTNodeBase.h>

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

