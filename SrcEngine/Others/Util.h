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

	//percentage %�̊m����true��Ԃ�
	bool Chance(int32_t percentage);

	/*
	�������Œ�l�ƍō��l��ݒ肵�ĕԂ��B
	RNG(1,3)�Ȃ�1, 2, 3�̂ǂꂩ���Ԃ�BpreciseMode��true�ɂ���Ƃ�萸���ɗ��������B
	*/
	int32_t RNG(int32_t min, int32_t max, bool preciseMode = false);

	std::wstring StrToWStr(const std::string& str, int32_t page = CP_ACP);

	//serialize�֐����`�����I�u�W�F�N�g�̃t�@�C���ۑ�/�ǂݍ���
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

