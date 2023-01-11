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

	//percentage %�̊m����true��Ԃ�
	bool Chance(int percentage);

	/*
	�������Œ�l�ƍō��l��ݒ肵�ĕԂ��B
	RNG(1,3)�Ȃ�1, 2, 3�̂ǂꂩ���Ԃ�BpreciseMode��true�ɂ���Ƃ�萸���ɗ��������B
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
