#include "stdafx.h"
#include "CustomComponentRegisterer.h"
//Engine Side
#include <PointLight.h>
#include <Object3D.h>
#include <BehaviorTree.h>
#include <IScene.h>
#include <Camera.h>
#include <StringObject.h>
#include <ScriptComponent.h>
#include <SpriteObject.h>

//Application Side
#include <TestMoveObject.h>

void CustomComponentRegisterer::CallRegisters()
{
	//Engine Side
	PointLight::RegisterToComponentFactory();
	Object3D::RegisterToComponentFactory();
	BT::BehaviorTree::RegisterToComponentFactory();
	Camera::RegisterToComponentFactory();
	StringObject::RegisterToComponentFactory();
	ScriptComponent::RegisterToComponentFactory();
	SpriteObject::RegisterToComponentFactory();

	//Application Side
	TestMoveObject::RegisterToComponentFactory();
}
