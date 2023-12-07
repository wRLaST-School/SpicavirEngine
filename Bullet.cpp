#include "stdafx.h"
#include "Bullet.h"

void Bullet::Init()
{
}

void Bullet::Update()
{
	timer++;

	if (timer >= 600)
	{
		parent_->RemoveComponent(this);
	}
}

void Bullet::Draw()
{
}
