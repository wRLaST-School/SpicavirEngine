#pragma once
#include <string>
#include <windows.h>
#include <fstream>
#include <cereal/archives/binary.hpp>

namespace Util
{
	template<class T>
	T Clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template<class T>
	T ClampMin(T value, T min)
	{
		return value < min ? min : value;
	}

	template<class T>
	T ClampMax(T value, T max)
	{
		return value > max ? max : value;
	}

	//percentage %の確率でtrueを返す
	bool Chance(int32_t percentage);

	/*
	乱数を最低値と最高値を設定して返す。
	RNG(1,3)なら1, 2, 3のどれかが返る。preciseModeをtrueにするとより精密に乱数を取る。
	*/
	int32_t RNG(int32_t min, int32_t max, bool preciseMode = false);

	std::wstring StrToWStr(const std::string& str, int32_t page = CP_ACP);

	//serialize関数を定義したオブジェクトのファイル保存/読み込み
	template<class T>
	void SerializeData(const std::string& path, const T& obj)
	{
		std::ofstream ofs(path.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive archive(ofs);
		archive(obj);
	}

	template<class T>
	void DeserializeData(const std::string& path, T& obj)
	{
		std::ifstream ifs(path.c_str(), std::ios::binary);
		cereal::BinaryInputArchive archive(ifs);
		archive(obj);
	}
};

struct R8G8B8A8
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 0;
};

