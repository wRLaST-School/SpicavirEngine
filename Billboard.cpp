#include "Billboard.h"

void Billboard::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	mRot = Matrix::RotRollPitchYaw(rotation);

	mTrans = Matrix::Translation(position);

	matWorld = Matrix::Identity();
	matWorld *= Matrix::RotRollPitchYaw(camera->rotation);
	matWorld *= mScale;
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

}
