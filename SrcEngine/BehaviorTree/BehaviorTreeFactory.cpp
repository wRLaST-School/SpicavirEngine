#include "stdafx.h"
#include "BehaviorTreeFactory.h"

void BT::BehaviorTreeFactory::RegisterCondition(std::string key, std::function<bool(void)> func)
{
	conditionTable_.emplace(key, func);
}

void BT::BehaviorTreeFactory::RegisterAction(std::string key, std::function<BT::Status(void)> func)
{
	actionTable_.emplace(key, func);
}

const std::function<BT::Status(void)> BT::BehaviorTreeFactory::GetActionFunc(const std::string& key)
{
	return actionTable_.at(key);
}

const std::function<bool(void)> BT::BehaviorTreeFactory::GetConditionFunc(const std::string& key)
{
	return conditionTable_.at(key);
}
