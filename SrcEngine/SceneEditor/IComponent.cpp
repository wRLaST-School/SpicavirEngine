#include "stdafx.h"
#include "IComponent.h"

void IComponent::RemoveComponent(std::string key)
{
	components_.erase(components_.find(key));
}

void IComponent::RemoveComponent(IComponent* ptr)
{
	for (auto itr = components_.begin(); itr != components_.end(); itr++)
	{
		if (itr->second.get() == ptr)
		{
			components_.erase(itr);
			return;
		}
	}
}

void IComponent::ClearComponentWithKey(std::string key)
{
	components_.erase(key);
}

IComponent* IComponent::GetComponent(std::string key)
{
	return components_.find(key)->second.get();
}

eastl::list<IComponent*> IComponent::GetComponents(std::string key)
{
	eastl::list<IComponent*> hitComponents;

	auto count = components_.count(key);
	auto itr = components_.find(key);
	for (size_t i = 0; i < count; i++)
	{
		hitComponents.emplace_back(itr->second.get());
		itr++;
	}

	return hitComponents;
}

const std::string& IComponent::GetName()
{
	return name_;
}

void IComponent::Update()
{
}

void IComponent::Draw()
{
}

void IComponent::DrawParams()
{
}




