#include "Matrix.h"

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
	return c[index];
}

float& Matrix::Row::operator[](size_t index)
{
	return c[index];
}

void Matrix::Row::operator=(const Float4& f4)
{
	c[0] = f4.x;
	c[1] = f4.y;
	c[2] = f4.z;
	c[3] = f4.w;
}

Matrix::Row Matrix::operator[](size_t index) const
{
	return r[index];
}

Matrix::Row& Matrix::operator[](size_t index)
{
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

Matrix Matrix::Identity()
{
	return Matrix();
}

Matrix Matrix::ViewLookAt(Float3 eye, Float3 target, Vec3 up)
{
	Vec3 camRZ = (Vec3)target - eye;
	Vec3 camRX = up.Cross(camRZ);
	Vec3 camRY = camRX.Cross(camRZ);

	Float4 result[4] = {
		{camRX.x, camRX.y, camRX.z, 0},
		{camRY.x, camRY.y, camRY.z, 0},
		{camRZ.x, camRZ.y, camRZ.z, 0},
		{camRX.Dot(eye), camRY.Dot(eye), camRZ.Dot(eye), 1}
	};

	return -Matrix(result);
}

Matrix Matrix::View(Matrix camera)
{
	return -camera;
}
