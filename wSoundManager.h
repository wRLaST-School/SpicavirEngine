#pragma once
#include "Essentials.h"
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <fstream>

typedef string SoundKey;

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

class SoundData
{
public:
	WAVEFORMATEX wfex;

	BYTE* pBuffer;

	unsigned int bufferSize;

	void Release() {
		delete[] pBuffer;

		this->pBuffer = 0;
		this->bufferSize = 0;
		this->wfex = {};
	}
};

class wSoundManager
{

public:
	static wSoundManager* GetInstance();
	static void Init();

	static SoundKey LoadWave(string path, SoundKey key);
	static void Play(SoundKey key);
	static IXAudio2SourceVoice* PlayBGM(SoundKey key);

	static void ReleaseAllSounds();

private:
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static map<SoundKey, SoundData> sndMap;


};