#pragma once
#include "Light.h"
#include <IComponent.h>
#include <ComponentFactory.h>
class IScene : public IComponent
{
public:
	ComponentFactoryRegister(IScene)

	IScene() { 
		name_ = "Scene";
	};

	virtual void LoadResources() {};
	virtual void Init() {};
	virtual void Update() {};
	virtual void DrawBack() {};
	virtual void Draw3D() {};
	virtual void DrawSprite() {};

	void UpdateAllComponents();
	void DrawAllComponents();

	virtual ~IScene() { 
	};
};

