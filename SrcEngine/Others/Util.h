#pragma once
/*****************************************************************//**
 * @file   Util.h
 * @brief  便利機能が詰まったヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include <string>
#include <windows.h>
#include <fstream>
#include <cereal/archives/binary.hpp>

/**
 * @brief 便利機能が利用できる
 */
namespace Util
{
	/**
	 * @brief 値を範囲内にクランプ
	 * 
	 * @param value クランプ元の値
	 * @param min 最低値
	 * @param max 最大値
	 * @return クランプされた値
	 */
	template<class T>
	T DLLExport Clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}

	/**
	 * @brief 値を範囲内にクランプ
	 *
	 * @param value クランプ元の値
	 * @param min 最低値
	 * @return クランプされた値
	 */
	template<class T>
	T DLLExport ClampMin(T value, T min)
	{
		return value < min ? min : value;
	}

	/**
	 * @brief 値を範囲内にクランプ
	 *
	 * @param value クランプ元の値
	 * @param max 最大値
	 * @return クランプされた値
	 */
	template<class T>
	T DLLExport ClampMax(T value, T max)
	{
		return value > max ? max : value;
	}

	/**
	 * @brief 確率でtrueになる
	 * 
	 * @return percentage %の確率でtrue
	 */
	bool DLLExport Chance(int32_t percentage);

	/**
	 * @brief 乱数を最低値と最高値を設定して取得
	 * 
	 * @param min 最低値
	 * @param max 最大値
	 * @param preciseMode trueにするとより精密に乱数を取る
	 * @return RNG(1,3)なら1, 2, 3のどれかが返る
	 */
	int32_t DLLExport RNG(int32_t min, int32_t max, bool preciseMode = false);

	/**
	 * @brief stringからwstringへの変換
	 */
	std::wstring DLLExport StrToWStr(const std::string& str, uint32_t page = CP_ACP);

	//serialize関数を定義したオブジェクトのファイル保存/読み込み
	template<class T>
	void DLLExport SerializeData(const std::string& path, const T& obj)
	{
		std::ofstream ofs(path.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive archive(ofs);
		archive(obj);
	}

	template<class T>
	void DLLExport DeserializeData(const std::string& path, T& obj)
	{
		std::ifstream ifs(path.c_str(), std::ios::binary);
		cereal::BinaryInputArchive archive(ifs);
		archive(obj);
	}

	Float2 DLLExport GetWinSize();
	int32_t DLLExport GetWinWidth();
	int32_t DLLExport GetWinHeight();
};

struct R8G8B8A8
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 0;
};

