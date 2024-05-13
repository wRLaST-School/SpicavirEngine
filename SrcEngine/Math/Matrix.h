#pragma once
/*****************************************************************//**
 * @file   Matrix.h
 * @brief  4x4の行列のヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include "Float4.h"
#include "Float3.h"
#include "Float2.h"
#include <Vec3.h>
#include <DLLDef.h>

/**
 * @briel 4x4行列のクラス
 */
class DLLExport Matrix
{
private:
	class DLLExport Row {
		float c[4];
	public:
		float operator[](size_t index) const;
		float& operator[](size_t index);
		void operator = (const Float4& f4);
	};

public:
//Constructors
	//単位行列で初期化
	Matrix();

	//Float4を4つで初期化
	Matrix(Float4 r0, Float4 r1, Float4 r2, Float4 r3);
	
	//Float4の配列で初期化
	Matrix(const Float4* pf44Array);

	//floatを16個で初期化
	Matrix(float r0c0, float r0c1, float r0c2, float r0c3,
		float r1c0, float r1c1, float r1c2, float r1c3,
		float r2c0, float r2c1, float r2c2, float r2c3,
		float r3c0, float r3c1, float r3c2, float r3c3);

	//floatの配列で初期化
	Matrix(const float* pf16Array);

//Operators
	Matrix operator - () const;

	Matrix operator + (const Matrix& m) const;
	Matrix operator - (const Matrix& m) const;
	Matrix operator * (const Matrix& m) const;

	Matrix& operator += (const Matrix& m);
	Matrix& operator -= (const Matrix& m);
	Matrix& operator *= (const Matrix& m);

	Row operator[](size_t index) const;
	Row& operator[](size_t index);

//Functions
	Matrix& Transpose();
	Matrix GetTranspose() const;

	//行列からオブジェクトのX軸方向のベクトルを取得
	Vec3 ExtractAxisX(float scale = 1.0);
	//行列からオブジェクトのY軸方向のベクトルを取得
	Vec3 ExtractAxisY(float scale = 1.0);
	//行列からオブジェクトのZ軸方向のベクトルを取得
	Vec3 ExtractAxisZ(float scale = 1.0);

	//Transform行列からPos,Rot,Scaleを抽出
	void DecomposeTransform(Float3* pos, Float3* rot, Float3* scale) const;

//Static Functions
	static Matrix Identity();

	static Matrix ViewLookTo(Float3 eyePos, Vec3 zVec, Vec3 up = Vec3(0.0f, 1.0f, 0.0f));
	static Matrix ViewLookAt(Float3 eye, Float3 target, Vec3 up = Vec3(0.0f, 1.0f, 0.0f));
	//逆行列を取得するだけ(カメラの行列からビューに変換)
	static Matrix View(Matrix camera);

	static Matrix Projection(float fov, float aspectRatio, float nearZ, float farZ);
	static Matrix Projection(int32_t windowWidth, int32_t windowHeight);
	static Matrix ProjectionOrtho(int32_t width, int32_t height, float nearZ, float farZ, float multiplier = 1.0f);

	static Matrix RotX(float rad);
	static Matrix RotY(float rad);
	static Matrix RotZ(float rad);

	//RotX
	static Matrix Pitch(float rad);
	//RotY
	static Matrix Yaw(float rad);
	//RotZ
	static Matrix Roll(float rad);

	static Matrix RotRollPitchYaw(float roll, float pitch, float yaw);
	static Matrix RotRollPitchYaw(Float3 pitchYawRoll);

	//任意軸回転(クォータニオン)
	static Matrix RotArbitrary(Vec3 axis, float rad);

	static Matrix Scale(Float3 scale);

	static Matrix Translation(Float3 t);

private:
	Row r[4];
};


Vec3& operator *=(Vec3& v, Matrix m);