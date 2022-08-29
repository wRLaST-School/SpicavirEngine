#pragma once
#include <string>
#include <windows.h>
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

	std::wstring StrToWStr(std::string str, int page = CP_ACP);

	bool Chance(int percentage);
	/*
	乱数を最低値と最高値を設定して返す。
	RNG(1,3)なら1, 2, 3のどれかが返る。preciseModeをtrueにするとより精密に乱数を取る。
	例えば、RAND_MAXの数値が100の環境で0-49の乱数を取ると0だけ確率が1.5倍(1, 51のとき1に対して0, 50, 100のとき0)になってしまうが、
	preciseModeではこの場合100が出たらもう一度乱数を生成しなおすことで全ての数字が同じ確率で出るようにしている。
	*/
	int RNG(int min, int max, bool preciseMode = false);
};

struct R8G8B8A8
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
	byte a = 0;
};

