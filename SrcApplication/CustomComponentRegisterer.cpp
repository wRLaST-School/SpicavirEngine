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
#include <NetworkManager.h>
#include <ServerPlayer.h>
#include <Circle.h>
#include <Bullet.h>
#include <ClientPlayer.h>

void CustomComponentRegisterer::CallRegisters()
{
	//Engine Side
	PointLight::RegisterToComponentFactory();
	Object3D::RegisterToComponentFactory();
	BT::BehaviorTree::RegisterToComponentFactory();
	LevelManager::RegisterToComponentFactory();
	Camera::RegisterToComponentFactory();

	//Application Side
	NetworkManager::RegisterToComponentFactory();
	ServerPlayer::RegisterToComponentFactory();
	CircleCollider::RegisterToComponentFactory();
	ClientPlayer::RegisterToComponentFactory();
	Bullet::RegisterToComponentFactory();
}
