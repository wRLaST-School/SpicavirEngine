#pragma once
#include <DirectXMath.h>

struct Float3;

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float x, float y, float z);

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	Vec3& SetLength(float l);

	Vec3 operator+(const Vec3& v2) const;

	Vec3 operator-(const Vec3& v2) const;

	Vec3 operator*(float mul) const;

	Vec3 operator/(float div) const;

	Vec3& operator +=(const Vec3& v2);

	Vec3& operator -=(const Vec3& v2);

	Vec3& operator *=(float mul);

	Vec3& operator /=(float div);

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	Vec3& Norm();

	//Sqrt‚ªŒÄ‚Î‚ê‚é
	Vec3 GetNorm() const;

	float GetLength() const;

	float GetSquaredLength() const;

	DirectX::XMFLOAT3 GetXMFloat() const;

	void AddToMatLH(DirectX::XMMATRIX mat);

	float Dot(const Vec3& v2) const;
	Vec3 Cross(const Vec3& v2) const;

	operator Float3() const;
};

Vec3 XMFloatToVec3(DirectX::XMFLOAT3 mat);

