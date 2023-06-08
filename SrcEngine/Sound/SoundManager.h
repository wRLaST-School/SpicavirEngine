#pragma once
#include "Essentials.h"
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <fstream>
#include <exc_unordered_map.h>

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

	uint32_t bufferSize;

	IXAudio2SourceVoice* sound;

	void Release() {
		delete[] pBuffer;

		this->pBuffer = 0;
		this->bufferSize = 0;
		this->wfex = {};

		if (sound != nullptr)
		{
			sound->Stop();
		}
	}
};

class SoundManager {
public:
	static SoundManager* GetInstance();
	static void Init();

	static SoundKey LoadWave(string path, SoundKey key);
	static void Play(SoundKey key);
	static SoundData* PlayBGM(SoundKey key, bool loopFlag);

	static SoundData* GetSoundData(SoundKey key);
	static void StopBGM(SoundKey key);

	static void ReleaseAllSounds();
	static void ReleasePerSceneSounds();
	static void PreLoadNewScene();

private:
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static exc_unordered_map<SoundKey, SoundData> sndMap;

private:
	static list<SoundKey> perSceneSounds[2];
	static int32_t currentSceneResIndex;

};