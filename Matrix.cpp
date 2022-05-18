#include "Matrix.h"

Matrix::Matrix()
{
}

float Matrix::Row::operator[](size_t index) const
{
	return c[index];
}

float& Matrix::Row::operator[](size_t index)
{
	return c[index];
}

Matrix::Row Matrix::operator[](size_t index) const
{
	return r[index];
}

Matrix::Row& Matrix::operator[](size_t index)
{
	return r[index];
}
