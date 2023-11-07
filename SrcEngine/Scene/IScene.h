#pragma once
#include "Light.h"
#include <IComponent.h>
class IScene : public IComponent
{
public:
	IScene() { 
		name_ = "Scene";
	};

	virtual void LoadResources() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawBack() = 0;
	virtual void Draw3D() = 0;
	virtual void DrawSprite() = 0;

	void UpdateAllComponents();
	void DrawAllComponents();

	virtual ~IScene() { 
	};
};

