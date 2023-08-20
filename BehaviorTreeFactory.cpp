#include "stdafx.h"
#include "BehaviorTreeFactory.h"

void BT::BehaviorTreeFactory::RegisterCondition(std::string key, std::function<bool(void)> func)
{
	conditionTable_.emplace(key, func);
}

void BT::BehaviorTreeFactory::RegisterAction(std::string key, std::function<void(void)> func)
{
	actionTable_.emplace(key, func);
}
