#pragma once
#include <Essentials.h>

class ComponentFactory
{
public:
	static IComponent* AddChildComponent(IComponent* parent, std::string key, std::string newComponentTypeStr);
	static void Register(std::string type, std::function<eastl::unique_ptr<IComponent>(void)> createFunc);

private:
	eastl::unordered_map<std::string, std::function<eastl::unique_ptr<IComponent>()>> factory;

public://singleton
	ComponentFactory(const ComponentFactory&) = delete;
	ComponentFactory(ComponentFactory&&) = delete;
	ComponentFactory& operator=(const ComponentFactory&) = delete;
	ComponentFactory& operator=(ComponentFactory&&) = delete;

	static ComponentFactory* GetInstance();

private:
	ComponentFactory() = default;
	~ComponentFactory() = default;
};

