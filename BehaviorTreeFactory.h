#pragma once
#include <EASTL/unordered_map.h>
#include <BTNodeBase.h>
namespace BT {
	class BehaviorTreeFactory
	{
	public:
		void RegisterCondition(std::string, std::function<bool(void)>);
		void RegisterAction(std::string, std::function<BT::Status(void)>);

		const std::function<BT::Status(void)> GetActionFunc(const std::string& key);
		const std::function<bool(void)> GetConditionFunc(const std::string& key);

	private:
		eastl::unordered_map<std::string, std::function<bool(void)>> conditionTable_;
		eastl::unordered_map<std::string, std::function<BT::Status(void)>> actionTable_;
	};
}

