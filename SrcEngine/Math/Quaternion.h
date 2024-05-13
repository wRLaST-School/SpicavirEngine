#pragma once
/*****************************************************************//**
 * @file   Quaternion.h
 * @brief  クォータニオンのヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include <Vec3.h>
/**
 * @briel クォータニオンクラス
 */
class DLLExport Quaternion
{
public:
	float w;
	Vec3 v;
public:
	Quaternion();
	/**
	 * @brief 軸ベクトルと回転量からクォータニオンを生成
	 * 
	 * @param axis 軸ベクトル
	 * @param rad 回転量
	 */
	Quaternion(Vec3 axis, float rad);

	/**
	 * @brief 実部と虚部からクォータニオンを生成
	 * 
	 * @param real 実部の数値
	 * @param imaginary 虚部の数値
	 */
	Quaternion(float real, Vec3 imaginary);

	/**
	 * @brief 軸のみを指定してクォータニオンを生成(回転量は0)
	 * 
	 * @param v 軸ベクトル
	 */
	Quaternion(Vec3 v);

	Quaternion operator* (const Quaternion& o) const;
	Quaternion operator* (const float& o) const;
	Quaternion& operator*= (const Quaternion& o);
	bool operator == (const Quaternion& o) const;

	/**
	 * @brief ノルムを取得
	 * 
	 * @return ノルム
	 */
	float GetNorm() const;

	/**
	 * @brief クォータニオンを正規化
	 * 
	 * @return 自身への参照
	 */
	Quaternion& Normalize();

	/**
	 * @brief 逆クォータニオン化する
	 * 
	 * @return 自身への参照
	 */
	Quaternion& Inverse();

	/**
	 * @brief クォータニオンをもとに回転行列を生成
	 * 
	 * @return 生成された回転行列 
	 */
	Matrix GetRotMat() const;

	float Dot(const Quaternion& o) const;
private:

public:
	/**
	 * @brief 単位クォータニオンを取得
	 * 
	 * @return 単位クォータニオン
	 */
	static Quaternion Identity();

	/**
	 * @brief 共役を返す
	 * 
	 * @param q 元のクォータニオン
	 * @return 共役
	 */
	static Quaternion Conjugate(const Quaternion& q);

	/**
	 * @brief 正規化したクォータニオンを取得
	 * 
	 * @param q 元のクォータニオン
	 * @return 正規化されたクォータニオン
	 */
	static Quaternion GetNormalized(const Quaternion& q);

	/**
	 * @brief 逆クォータニオンを取得
	 * 
	 * @param q 元のクォータニオン
	 * @return 逆クォータニオン
	 */
	static Quaternion GetInverse(const Quaternion& q);

	/**
	 * @brief 2つの回転クォータニオンをtの値でSlerpする
	 * 
	 * @param zero 始点
	 * @param one 終点
	 * @param t 時間t(0.f - 1.f)
	 * @return 時間tにおけるクォータニオン
	 */
	static Quaternion Slerp(const Quaternion& zero, const Quaternion& one, const float& t);
	
	/**
	 * @brief fromベクトルをtoベクトル方向に変換するための回転クォータニオンを取得
	 * 
	 * @param from 回転元
	 * @param to 回転後
	 * @return 回転クォータニオン
	 */
	static Quaternion DirToDir(const Vec3& from, const Vec3& to);

	/**
	 * @brief fromベクトルをtoベクトル方向に変換するための回転クォータニオンを取得
	 * @details 最大回転量を制限することで、緩いホーミング弾等に利用可能
	 * 
	 * @param from 回転元
	 * @param to 回転後
	 * @param maxRad 最大回転量
	 * @return 回転クォータニオン
	 */
	static Quaternion DirToDir(const Vec3& from, const Vec3& to, const float maxRad);
	
	/**
	 * @brief オイラー角回転からクォータニオンを生成
	 * 
	 * @param xyz 各軸の回転量
	 * @return 回転クォータニオン
	 */
	static Quaternion EulerToQuaternion(const Float3& xyz);
};

Vec3 operator *(Vec3 v, Quaternion q);
