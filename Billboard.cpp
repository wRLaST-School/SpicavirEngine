#include "Billboard.h"

void Billboard::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	mRot = Matrix::RotRollPitchYaw(rotation);

	mTrans = Matrix::Translation(position);

	matWorld = Matrix::Identity();
	matWorld *= mScale;
	matWorld *= Matrix::RotRollPitchYaw(camera->rotation);//ビルボード回転
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

}

void BillboardY::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans, mYBill;

	mScale = Matrix::Scale(scale);

	mRot = Matrix::RotRollPitchYaw(rotation);

	mTrans = Matrix::Translation(position);

	mYBill = Matrix::Identity();
	mYBill[0] = (Float4)(Float3)camera->matWorld.ExtractAxisX();
	mYBill[1] = (Float4)(Float3)upVec;
	mYBill[2] = (Float4)(Float3)camera->matWorld.ExtractAxisX().Cross(upVec);

	matWorld = Matrix::Identity();
	matWorld *= mScale;
	matWorld *= mYBill;//ビルボード回転
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
}
