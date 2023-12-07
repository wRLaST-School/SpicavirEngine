#include "stdafx.h"
#include "IComponent.h"

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)

using namespace nlohmann;

IComponent* IComponent::AddComponent(const std::string& key, eastl::unique_ptr<IComponent> component)
{
	auto itr = components_.insert(eastl::make_pair(key, eastl::move(component)));
	itr->second->name_ = itr->first;
	itr->second->parent_ = this;

	return itr->second.get();
}

void IComponent::ChangeParent(IComponent* newParent)
{
	for (auto itr = parent_->components_.begin(); itr != parent_->components_.end(); itr++)
	{
		if ((*itr).second.get() == this)
		{
			//newParent->components_.emplace(name_, std::move((*itr)));
			newParent;
			parent_->components_.erase(itr);

			return;
		}
	}
}

void IComponent::RemoveComponent(const std::string& key)
{
	childRemovedNewItr_ = components_.erase(components_.find(key));
}

void IComponent::RemoveComponent(IComponent* ptr)
{
	for (auto itr = components_.begin(); itr != components_.end(); itr++)
	{
		if (itr->second.get() == ptr)
		{
			childRemovedNewItr_ = components_.erase(itr);
			return;
		}
	}
}

void IComponent::ClearComponentWithKey(const std::string& key)
{
	components_.erase(key);
}

void IComponent::ClearAllComponents()
{
	components_.clear();
}

IComponent* IComponent::GetComponent(const std::string& key)
{
	return components_.find(key)->second.get();
}

eastl::list<IComponent*> IComponent::GetComponents(const std::string& key)
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

const eastl::multimap<std::string, eastl::unique_ptr<IComponent>>& IComponent::GetAllComponents()
{
	return components_;
}

const std::string& IComponent::GetName()
{
	return name_;
}

void IComponent::Init()
{
}

void IComponent::Update()
{
}

void IComponent::Draw()
{
}

void IComponent::InitAllChildComponents(IComponent* parent)
{
	parent->Init();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			InitAllChildComponents(c.second.get());
		}
	}
}

void IComponent::UpdateAllChildComponents(IComponent* parent)
{
	parent->childRemovedNewItr_ = parent->components_.begin();

	if (parent->components_.size())
	{
		for (auto itr = parent->components_.begin(); itr != parent->components_.end();)
		{
			UpdateAllChildComponents(itr->second.get());
			if (parent->childRemovedNewItr_ != parent->components_.begin()) // beginでもendでもないitrに変更したい
			{
				itr = parent->childRemovedNewItr_;
				parent->childRemovedNewItr_ = parent->components_.begin();
			}
			else
			{
				itr++;
			}
		}
	}

	parent->Update();
}

void IComponent::DrawAllChildComponents(IComponent* parent)
{
	parent->Draw();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			DrawAllChildComponents(c.second.get());
		}
	}
}

void IComponent::DrawParams()
{
}




