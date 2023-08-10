#include "stdafx.h"
#include "GravSphere.h"
#include <Player.h>

GravSphere::GravSphere(Float3 pos, Vec3 vel, float speed)
{
	pos_ = pos;
	vel_ = vel;
	speed_ = speed;
	hnd_ = SpEffekseer::Play("SphereParticle", pos_);

	sphere_.model = ModelManager::GetModel("Sphere");
}

void GravSphere::Update()
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

	sphere_.UpdateMatrix();
}

void GravSphere::Draw()
{
	sphere_.DrawAlpha("white");
	SpEffekseer::Manager()->SetLocation(hnd_, Effekseer::Vector3D(pos_.x, pos_.y, pos_.z));
}
