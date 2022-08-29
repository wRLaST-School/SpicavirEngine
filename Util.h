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
	�������Œ�l�ƍō��l��ݒ肵�ĕԂ��B
	RNG(1,3)�Ȃ�1, 2, 3�̂ǂꂩ���Ԃ�BpreciseMode��true�ɂ���Ƃ�萸���ɗ��������B
	�Ⴆ�΁ARAND_MAX�̐��l��100�̊���0-49�̗���������0�����m����1.5�{(1, 51�̂Ƃ�1�ɑ΂���0, 50, 100�̂Ƃ�0)�ɂȂ��Ă��܂����A
	preciseMode�ł͂��̏ꍇ100���o���������x�����𐶐����Ȃ������ƂőS�Ă̐����������m���ŏo��悤�ɂ��Ă���B
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

