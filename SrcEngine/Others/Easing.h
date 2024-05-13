#pragma once
/*****************************************************************//**
 * @file   Easing.h
 * @brief  イージングに関する関数のヘッダー
 * 
 * @author Wrelf
 *********************************************************************/

/**
 * @brief イージング関連の関数がまとまっているクラス
 */
class DLLExport Easing {
public:
	/**
	 * @brief 累乗を利用したEaseInの関数
	 * 
	 * @param t 時間t
	 * @param exp 何乗で求めるか
	 * @return イージングのかかったt
	 */
	static float In(float t, float exp = 3);

	/**
	 * @brief 累乗を利用したEaseOutの関数
	 *
	 * @param t 時間t
	 * @param exp 何乗で求めるか
	 * @return イージングのかかったt
	 */
	static float Out(float t, float exp = 3);

	/**
	 * @brief 累乗を利用したEaseInで2点の間の座標を求める
	 * 
	 * @param zero 始点
	 * @param one 終点
	 * @param t 時間t
	 * @param exp 何乗で求めるか
	 * @return 時間tでの座標
	 */
	static Vec3 In(Vec3 zero, Vec3 one, float t, float exp = 3);

	/**
	 * @brief 累乗を利用したEaseOutで2点の間の座標を求める
	 *
	 * @param zero 始点
	 * @param one 終点
	 * @param t 時間t
	 * @param exp 何乗で求めるか
	 * @return 時間tでの座標
	 */
	static Vec3 Out(Vec3 zero, Vec3 one, float t, float exp = 3);
};

