#include "Matrix.h"
#include <math.h>
#include <stdexcept>

Matrix::Matrix():Matrix(1.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 1.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 1.0f)
{

}

Matrix::Matrix(Float4 r0, Float4 r1, Float4 r2, Float4 r3)
{
	r[0] = r0;
	r[1] = r1;
	r[2] = r2;
	r[3] = r3;
}

Matrix::Matrix(const Float4* pf44Array):Matrix(pf44Array[0], pf44Array[1], pf44Array[2], pf44Array[3])
{
}

Matrix::Matrix(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3)
{
	r[0][0] = r0c0;
	r[0][1] = r0c1;
	r[0][2] = r0c2;
	r[0][3] = r0c3;

	r[1][0] = r1c0;
	r[1][1] = r1c1;
	r[1][2] = r1c2;
	r[1][3] = r1c3;

	r[2][0] = r2c0;
	r[2][1] = r2c1;
	r[2][2] = r2c2;
	r[2][3] = r2c3;

	r[3][0] = r3c0;
	r[3][1] = r3c1;
	r[3][2] = r3c2;
	r[3][3] = r3c3;
}

Matrix::Matrix(const float* pf16Array)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			r[i][j] = *pf16Array;
			pf16Array++;
		}
	}
}

float Matrix::Row::operator[](size_t index) const
{
	if (index > 3) throw std::out_of_range("ITS 4 x 4 MATRIX");
	return c[index];
}

float& Matrix::Row::operator[](size_t index)
{
	if (index > 3) throw std::out_of_range("ITS 4 x 4 MATRIX");
	return c[index];
}

void Matrix::Row::operator=(const Float4& f4)
{
	c[0] = f4.x;
	c[1] = f4.y;
	c[2] = f4.z;
	c[3] = f4.w;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
	*this = *this * m;
	return *this;
}

Matrix::Row Matrix::operator[](size_t index) const
{
	if (index > 3) throw std::out_of_range("ITS 4 x 4 MATRIX");
	return r[index];
}

Matrix::Row& Matrix::operator[](size_t index)
{
	if (index > 3) throw std::out_of_range("ITS 4 x 4 MATRIX");
	return r[index];
}

Matrix& Matrix::Transpose()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = i + 1; j < 4; j++)
		{
			float temp = r[i][j];
			r[i][j] = r[j][i];
			r[j][i] = temp;
		}
	}
	return *this;
}

Matrix Matrix::GetTranspose() const
{
	Matrix ret;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			ret[i][j] = r[j][i];
		}
	}
	return ret;
}

Vec3 Matrix::ExtractAxisX(float scale)
{
	return Vec3(r[0][0] / scale, r[0][1], r[0][2]).GetNorm();
}

Vec3 Matrix::ExtractAxisY(float scale)
{
	return Vec3(r[1][0], r[1][1] / scale, r[1][2]).GetNorm();
}

Vec3 Matrix::ExtractAxisZ(float scale)
{
	return Vec3(r[2][0], r[2][1], r[2][2] / scale).GetNorm();
}

Matrix Matrix::Identity()
{
	return Matrix();
}

Matrix Matrix::ViewLookTo(Float3 eyePos, Vec3 camRZ, Vec3 up)
{
	Vec3 camRX = up.Cross(camRZ).GetNorm();
	Vec3 camRY = camRZ.Cross(camRX).GetNorm();

	Matrix result = Matrix(
		camRX.x, camRX.y, camRX.z, 0,
		camRY.x, camRY.y, camRY.z, 0,
		camRZ.x, camRZ.y, camRZ.z, 0,
		eyePos.x, eyePos.y, eyePos.z, 1
	);

	return -result;
}

Matrix Matrix::ViewLookAt(Float3 eye, Float3 target, Vec3 up)
{
	Vec3 camRZ = (Vec3)target - eye;
	camRZ.Norm();
	Vec3 camRX = up.Cross(camRZ).GetNorm();
	Vec3 camRY = camRZ.Cross(camRX).GetNorm();

	Matrix result = Matrix(
		camRX.x, camRX.y, camRX.z, 0,
		camRY.x, camRY.y, camRY.z, 0,
		camRZ.x, camRZ.y, camRZ.z, 0,
		eye.x, eye.y, eye.z, 1
	);

	return -result;
}

Matrix Matrix::View(Matrix camera)
{
	return -camera;
}

Matrix Matrix::Projection(float fov, float aspectRatio, float nearZ, float farZ)
{
	Matrix result = Matrix();

	result[1][1] = 1 / tanf(fov / 2);
	result[0][0] = result[1][1] / aspectRatio;
	result[2][2] = farZ / (farZ - nearZ);
	result[3][2] = (farZ * -nearZ) / (farZ - nearZ);
	result[2][3] = 1;
	result[3][3] = 0;

	return result;
}

Matrix Matrix::Projection(int windowWidth, int windowHeight)
{
	return Matrix(
		2.0f / windowWidth, 0, 0, 0,
		0, -2.0f / windowHeight, 0, 0,
		0, 0, 1, 0,
		-1 , 1, 0, 1
	);
}

Matrix Matrix::RotX(float rad)
{
	return Pitch(rad);
}

Matrix Matrix::RotY(float rad)
{
	return Yaw(rad);
}

Matrix Matrix::RotZ(float rad)
{
	return Roll(rad);
}

Matrix Matrix::operator-() const
{
	Matrix result;
	float temp[4][8] = {};

	double a;

	//一時行列にコピー
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[i][j] = r[i][j];

			if(i == j)temp[i][4 + j] = 1;
		}
	}

	for (int k = 0; k < 4; k++) {
		a = 1 / temp[k][k];

		for (int j = 0; j < 8; j++) {
			temp[k][j] *= a;
		}

		for (int i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			a = -temp[i][k];

			for (int j = 0; j < 8; j++) {
				temp[i][j] += temp[k][j] * a;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = temp[i][4 + j];
		}
	}
	return result;
}

Matrix Matrix::operator+(const Matrix& m) const
{
	return Matrix(
		r[0][0] + m[0][0],
		r[0][1] + m[0][1],
		r[0][2] + m[0][2],
		r[0][3] + m[0][3],

		r[1][0] + m[1][0],
		r[1][1] + m[1][1],
		r[1][2] + m[1][2],
		r[1][3] + m[1][3],

		r[2][0] + m[2][0],
		r[2][1] + m[2][1],
		r[2][2] + m[2][2],
		r[2][3] + m[2][3],

		r[3][0] + m[3][0],
		r[3][1] + m[3][1],
		r[3][2] + m[3][2],
		r[3][3] + m[3][3]
	);
}

Matrix Matrix::operator-(const Matrix& m) const
{
	return Matrix(
		r[0][0] - m[0][0],
		r[0][1] - m[0][1],
		r[0][2] - m[0][2],
		r[0][3] - m[0][3],
				
		r[1][0] - m[1][0],
		r[1][1] - m[1][1],
		r[1][2] - m[1][2],
		r[1][3] - m[1][3],
				
		r[2][0] - m[2][0],
		r[2][1] - m[2][1],
		r[2][2] - m[2][2],
		r[2][3] - m[2][3],
				
		r[3][0] - m[3][0],
		r[3][1] - m[3][1],
		r[3][2] - m[3][2],
		r[3][3] - m[3][3]
	);
}

Matrix Matrix::operator*(const Matrix& m) const
{
	return Matrix(
		r[0][0] * m[0][0] + r[0][1] * m[1][0] + r[0][2] * m[2][0] + r[0][3] * m[3][0],
		r[0][0] * m[0][1] + r[0][1] * m[1][1] + r[0][2] * m[2][1] + r[0][3] * m[3][1],
		r[0][0] * m[0][2] + r[0][1] * m[1][2] + r[0][2] * m[2][2] + r[0][3] * m[3][2],
		r[0][0] * m[0][3] + r[0][1] * m[1][3] + r[0][2] * m[2][3] + r[0][3] * m[3][3],

		r[1][0] * m[0][0] + r[1][1] * m[1][0] + r[1][2] * m[2][0] + r[1][3] * m[3][0],
		r[1][0] * m[0][1] + r[1][1] * m[1][1] + r[1][2] * m[2][1] + r[1][3] * m[3][1],
		r[1][0] * m[0][2] + r[1][1] * m[1][2] + r[1][2] * m[2][2] + r[1][3] * m[3][2],
		r[1][0] * m[0][3] + r[1][1] * m[1][3] + r[1][2] * m[2][3] + r[1][3] * m[3][3],

		r[2][0] * m[0][0] + r[2][1] * m[1][0] + r[2][2] * m[2][0] + r[2][3] * m[3][0],
		r[2][0] * m[0][1] + r[2][1] * m[1][1] + r[2][2] * m[2][1] + r[2][3] * m[3][1],
		r[2][0] * m[0][2] + r[2][1] * m[1][2] + r[2][2] * m[2][2] + r[2][3] * m[3][2],
		r[2][0] * m[0][3] + r[2][1] * m[1][3] + r[2][2] * m[2][3] + r[2][3] * m[3][3],

		r[3][0] * m[0][0] + r[3][1] * m[1][0] + r[3][2] * m[2][0] + r[3][3] * m[3][0],
		r[3][0] * m[0][1] + r[3][1] * m[1][1] + r[3][2] * m[2][1] + r[3][3] * m[3][1],
		r[3][0] * m[0][2] + r[3][1] * m[1][2] + r[3][2] * m[2][2] + r[3][3] * m[3][2],
		r[3][0] * m[0][3] + r[3][1] * m[1][3] + r[3][2] * m[2][3] + r[3][3] * m[3][3]
	);
}

Matrix& Matrix::operator+=(const Matrix& m)
{
	r[0][0] += m[0][0];
	r[0][1] += m[0][1];
	r[0][2] += m[0][2];
	r[0][3] += m[0][3];

	r[1][0] += m[1][0];
	r[1][1] += m[1][1];
	r[1][2] += m[1][2];
	r[1][3] += m[1][3];

	r[2][0] += m[2][0];
	r[2][1] += m[2][1];
	r[2][2] += m[2][2];
	r[2][3] += m[2][3];
					  
	r[3][0] += m[3][0];
	r[3][1] += m[3][1];
	r[3][2] += m[3][2];
	r[3][3] += m[3][3];

	return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
	r[0][0] -= m[0][0];
	r[0][1] -= m[0][1];
	r[0][2] -= m[0][2];
	r[0][3] -= m[0][3];
			
	r[1][0] -= m[1][0];
	r[1][1] -= m[1][1];
	r[1][2] -= m[1][2];
	r[1][3] -= m[1][3];
			
	r[2][0] -= m[2][0];
	r[2][1] -= m[2][1];
	r[2][2] -= m[2][2];
	r[2][3] -= m[2][3];
			
	r[3][0] -= m[3][0];
	r[3][1] -= m[3][1];
	r[3][2] -= m[3][2];
	r[3][3] -= m[3][3];

	return *this;
}

Matrix Matrix::Pitch(float rad)
{
	Matrix result = Matrix();

	result[1][1] = cosf(rad);
	result[2][1] = -sinf(rad);
	result[1][2] = sinf(rad);
	result[2][2] = cosf(rad);

	return result;
}

Matrix Matrix::Yaw(float rad)
{
	Matrix result = Matrix();

	result[0][0] = cosf(rad);
	result[0][2] = -sinf(rad);
	result[2][0] = sinf(rad);
	result[2][2] = cosf(rad);

	return result;
}

Matrix Matrix::Roll(float rad)
{
	Matrix result = Matrix();

	result[0][0] = cosf(rad);
	result[1][0] = -sinf(rad);
	result[0][1] = sinf(rad);
	result[1][1] = cosf(rad);

	return result;
}

Matrix Matrix::RotRollPitchYaw(float roll, float pitch, float yaw)
{
	Matrix result = Matrix::Identity() * Roll(roll) * Pitch(pitch) * Yaw(yaw);
	return result;
}

Matrix Matrix::RotRollPitchYaw(Float3 pitchYawRoll)
{
	return RotRollPitchYaw(pitchYawRoll.z, pitchYawRoll.x, pitchYawRoll.y);
}

Matrix Matrix::RotArbitrary(Vec3 axis, float rad)
{
	float sn = sin(rad / 2);
	axis.x *= sn;
	axis.y *= sn;
	axis.z *= sn;
	float w = cos(rad / 2);
	Float4 X = { axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * w };
	Float3 Y = { axis.y * w, axis.y * axis.y, axis.y * axis.z };
	Float2 Z = { axis.z * axis.z, axis.z * w };
	return Matrix(
		1 - 2 * Y.y - 2 * Z.x,
		2 * X.y + 2 * Z.y,
		2 * X.z - 2 * Y.x,
		0,

		2 * X.y - 2 * Z.y,
		1 - 2 * X.x - 2*Z.x,
		2*Y.z + 2* X.w,
		0,

		2* X.z + 2* Y.x,
		2*Y.z - 2*X.w,
		1- 2*X.x - 2*Y.y,
		0,

		0, 0, 0, 1
	);
}

Matrix Matrix::Scale(Float3 scale)
{
	Matrix ret = Matrix();
	ret[0][0] = scale.x;
	ret[1][1] = scale.y;
	ret[2][2] = scale.z;

	return ret;
}

Matrix Matrix::Translation(Float3 t)
{
	Matrix ret = Matrix();
	ret[3][0] = t.x;
	ret[3][1] = t.y;
	ret[3][2] = t.z;

	return ret;
}
