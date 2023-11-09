#include "stdafx.h"
#include "CustomComponentRegisterer.h"
//Engine Side
#include <PointLight.h>
#include <Object3D.h>
#include <BehaviorTree.h>

//Application Side
#include <Player.h>
#include <Boss.h>


void CustomComponentRegisterer::CallRegisters()
{
	//Engine Side
	PointLight::RegisterToComponentFactory();
	Object3D::RegisterToComponentFactory();
	BT::BehaviorTree::RegisterToComponentFactory();

	//Application Side
	Player::RegisterToComponentFactory();
	Boss::RegisterToComponentFactory();
}
