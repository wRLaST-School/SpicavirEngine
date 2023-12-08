#include "stdafx.h"
#include "Bullet.h"
#include <SpDS.h>
#include <Circle.h>
#include <ServerPlayer.h>

Bullet::Bullet()
{
	AddComponent<CircleCollider>("Collider");
}

void Bullet::Init()
{
}

void Bullet::Update()
{
	if (!active)
	{
		pos_ = path[0];
		return;
	}

	if (path[pathIndex].x == -1000.f)
	{
		pathIndex = 1;
	}

	if (timer == 0)
	{
		this->pos_ = path[0];
		pathIndex++;
		while (!path[pathIndex].GetSquaredLength())
		{
			pathIndex++;
		}
		if (path[pathIndex].x == -1000.f)
		{
			pathIndex = 1;
		}
	}
	else
	{
		Vec2 tempVec = path[pathIndex];

		if (lastLength)
		{
			tempVec.SetLength(tempVec.GetLength() - lastLength);
		}

		float remainSpd = speed;
		while (remainSpd > 0)
		{
			float l = tempVec.GetLength();

			if (l > remainSpd)
			{
				tempVec.SetLength(remainSpd);
				pos_ = tempVec + pos_;

				lastLength += remainSpd;
				break;
			}
			else
			{
				remainSpd -= l;
				pos_ = tempVec + pos_;
				pathIndex++;

				size_t firstIndex = pathIndex;
				bool isAllZero = false;
				while (path[pathIndex].GetSquaredLength() == 0)
				{
					pathIndex++;
					if (pathIndex == firstIndex)
					{
						isAllZero = true;
						break;
					}
					if (path[pathIndex].x == -1000.f)
					{
						pathIndex = 1;
					}
				}
				
				tempVec = path[pathIndex];

				lastLength = 0;

				if (isAllZero) break;
			}
		}
	}

	timer++;

	GetComponent<CircleCollider>("Collider")->pos_ = this->pos_;
	GetComponent<CircleCollider>("Collider")->r_ = this->size;
	if (GetComponent<CircleCollider>("Collider")->Collide(
		*parent_->parent_->GetComponent("ServerPlayer")->GetComponent<CircleCollider>("CircleCollider")
	))
	{
		parent_->parent_->GetComponent<ServerPlayer>("ServerPlayer")->Damage();
		parent_->RemoveComponent(this);
		return;
	}

	if (timer >= 600)
	{
		parent_->RemoveComponent(this);
	}
}

void Bullet::Draw()
{
	SpDS::DrawRotaGraph((int)pos_.x, (int)pos_.y, size / 45, size / 45, 0.f, "bullet.png");
}

void Bullet::Activate()
{
	active = true;
}
