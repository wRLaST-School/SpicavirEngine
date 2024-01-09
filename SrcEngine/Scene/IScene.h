#pragma once
/*****************************************************************//**
 * @file   IScene.h
 * @brief  シーンをプログラム上で作成する場合の基底クラス
 * @details シーンエディタ作成後不要になったが、念のため残してある
 * 
 * @author Wrelf
 *********************************************************************/
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

