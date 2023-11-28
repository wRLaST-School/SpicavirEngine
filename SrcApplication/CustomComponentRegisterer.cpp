#include "stdafx.h"
#include "CustomComponentRegisterer.h"
//Engine Side
#include <PointLight.h>
#include <Object3D.h>
#include <BehaviorTree.h>
#include <IScene.h>
#include <LevelManager.h>
#include <Camera.h>

//Application Side
#include <Player.h>
#include <Boss.h>
#include <CameraController.h>
#include <MainTimer.h>

void CustomComponentRegisterer::CallRegisters()
{
	//Engine Side
	PointLight::RegisterToComponentFactory();
	Object3D::RegisterToComponentFactory();
	BT::BehaviorTree::RegisterToComponentFactory();
	LevelManager::RegisterToComponentFactory();
	Camera::RegisterToComponentFactory();

	//Application Side
	CameraController::RegisterToComponentFactory();
	Player::RegisterToComponentFactory();
	Boss::RegisterToComponentFactory();
	LineAttack::RegisterToComponentFactory();
	GravSphere::RegisterToComponentFactory();
	Marker::RegisterToComponentFactory();
	MainTimer::RegisterToComponentFactory();
}
