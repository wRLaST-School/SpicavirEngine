#pragma once
#include <EASTL/unordered_map.h>
#include <BTNodeBase.h>
namespace BT {
	class BehaviorTreeFactory
	{
	public:
		void RegisterCondition(std::string, std::function<bool(void)>);
		void RegisterAction(std::string, std::function<BT::Status(void)>);

	private:
		eastl::unordered_map<std::string, std::function<bool(void)>> conditionTable_;
		eastl::unordered_map<std::string, std::function<BT::Status(void)>> actionTable_;
	};
}

