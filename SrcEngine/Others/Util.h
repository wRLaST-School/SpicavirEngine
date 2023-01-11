#pragma once
#include <string>
#include <windows.h>
#include <DirectXPackedVector.h>
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
	bool Chance(int percentage);

	/*
	乱数を最低値と最高値を設定して返す。
	RNG(1,3)なら1, 2, 3のどれかが返る。preciseModeをtrueにするとより精密に乱数を取る。
	*/
	int RNG(int min, int max, bool preciseMode = false);

	std::wstring StrToWStr(std::string str, int page = CP_ACP);
};

struct R8G8B8A8
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 0;
};

struct R16G16B16A16_FLOAT
{
	 DirectX::PackedVector::HALF r = 0.f;
	 DirectX::PackedVector::HALF g = 0.f;
	 DirectX::PackedVector::HALF b = 0.f;
	 DirectX::PackedVector::HALF a = 0.f;
};
