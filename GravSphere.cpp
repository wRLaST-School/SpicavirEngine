#include "stdafx.h"
#include "GravSphere.h"
#include <Player.h>

GravSphere::GravSphere(const Float3& pos, const Vec3& vel, float speed,
	float gravR, float gravitySpeed, float maxHomeRad, int32_t stayTime)
{
	pos_ = pos;
	vel_ = vel;
	speed_ = speed;
	gravR_ = gravR;
	gravSpeed_ = gravitySpeed;
	maxHomeRad_ = maxHomeRad;
	stayTime_ = stayTime;

	hnd_ = SpEffekseer::Play("SphereParticle", pos_);

	sphere_.model = ModelManager::GetModel("Sphere");
}

void GravSphere::Update()
{
	timer_++;
	if (timer_ < stayTime_)
	{
		float t = (float)timer_ / stayTime_;
		float r = r_ * t;

		sphere_.scale = { r, r, r };
		sphere_.position = pos_;

		*sphere_.brightnessCB.contents = { 0.3f, 0.2f, 0.3f, 0.4f };
	}
	else
	{
		Vec3 target = (Vec3)Player::Get()->position - pos_;
		if (target.GetSquaredLength())
		{
			target.Norm();

			if (vel_.GetSquaredLength() == 0)
			{
				pos_ = target * speed_ + pos_;
			}
			else
			{
				Vec3 normVel = vel_.GetNorm();
				normVel = normVel * Quaternion::DirToDir(normVel, target, maxHomeRad_);
				vel_ = normVel * speed_;
				pos_ = vel_ + pos_;
			}
		}

		*sphere_.brightnessCB.contents = { 0.3f, 0.2f, 0.3f, 0.4f };

		sphere_.scale = { r_, r_, r_ };

		pos_.y = Util::ClampMin(pos_.y, r_ / 2.f);

		sphere_.position = pos_;

		CheckCollisions();
	}

	sphere_.UpdateMatrix();
}

void GravSphere::CheckCollisions()
{
	Player* pl = Player::Get();

	float playerDistSquared = ((Vec3)pl->GetCollider().pos - pos_).GetSquaredLength();

	//�d�͂̔���
	if (playerDistSquared <= gravR_ * gravR_)
	{
		Vec3 vel = (Vec3)pos_ - pl->GetCollider().pos;

		vel.y = 0;

		vel.SetLength(gravSpeed_);

		pl->MoveTo((Vec3)pl->position + vel);
	}


	//��e�̔���
	if (playerDistSquared <= r_ * r_)
	{
		pl->Damage();
	}
}

void GravSphere::Draw()
{
	sphere_.DrawAlpha("white");
	SpEffekseer::Manager()->SetLocation(hnd_, Effekseer::Vector3D(pos_.x, pos_.y, pos_.z));
}
