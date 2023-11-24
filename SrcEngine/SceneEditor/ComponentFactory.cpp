#include "stdafx.h"
#include "ComponentFactory.h"

IComponent* ComponentFactory::AddChildComponent(IComponent* parent, std::string key, std::string newComponentTypeStr)
{
	return parent->AddComponent(key, GetInstance()->factory.find(newComponentTypeStr)->second());
}

void ComponentFactory::Register(std::string type, std::function<eastl::unique_ptr<IComponent>()> createFunc)
{
    GetInstance()->factory.emplace(type, createFunc);
}

ComponentFactory* ComponentFactory::GetInstance()
{
    static ComponentFactory ins;
    return &ins;
}
