#pragma once
#include <IComponent.h>

class ScriptComponent;

class IScriptObject : public IComponent
{
public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnInspectorWindowDraw() {};

	ScriptComponent* This();

	std::string GetClassString() override;

	ScriptComponent* body;
};

