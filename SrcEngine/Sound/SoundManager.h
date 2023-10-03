#pragma once
#include "Essentials.h"
#include <xaudio2.h>

#include <fstream>
#include <exc_unordered_map.h>

typedef std::string SoundKey;

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
		if (sound != nullptr)
		{
			sound->Stop();
			sound->DestroyVoice();
		}

		delete[] pBuffer;

		this->pBuffer = 0;
		this->bufferSize = 0;
		this->wfex = {};
	}
};

class SoundManager {
public:
	static SoundManager* GetInstance();
	static void Init();

	static SoundKey LoadWave(const std::string& path, const SoundKey& key);
	static void Play(const SoundKey& key);
	static SoundData* PlayBGM(const SoundKey& key, bool loopFlag);

	static SoundData* GetSoundData(const SoundKey& key);
	static void StopBGM(const SoundKey& key);

	static void ReleaseAllSounds();
	static void ReleasePerSceneSounds();
	static void PreLoadNewScene();

private:
	static Microsoft::WRL::ComPtr<IXAudio2> sXAudio2;
	static IXAudio2MasteringVoice* sMasterVoice;
	static exc_unordered_map<SoundKey, SoundData> sSndMap;

private:
	static std::list<SoundKey> sPerSceneSounds[2];
	static int32_t sCurrentSceneResIndex;

};