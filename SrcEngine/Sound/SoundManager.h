#pragma once
/*****************************************************************//**
 * @file   SoundManager.h
 * @brief  サウンド読み込み・再生関連のヘッダー
 * 
 * @author Wrelf
 *********************************************************************/

#include "Essentials.h"

#pragma warning(push, 0)
#include <xaudio2.h>
#pragma warning(pop)

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

/**
 * @brief 音情報
 */
class SoundData
{
public:
	WAVEFORMATEX wfex;

	BYTE* pBuffer;

	uint32_t bufferSize;

	IXAudio2SourceVoice* sound;

	std::string filePath;

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

class SceneRW;

/**
 * @brief 音の読み込み・再生を行う
 */
class SoundManager {
public:
	//シーン情報書き込みで使用するため
	friend SceneRW;

	static SoundManager* GetInstance();
	static void Init();

	static DLLExport SoundKey LoadWave(const std::string& path, const SoundKey& key);
	static DLLExport void Play(const SoundKey& key);
	static DLLExport SoundData* PlayBGM(const SoundKey& key, bool loopFlag);

	static DLLExport SoundData* GetSoundData(const SoundKey& key);
	static DLLExport void StopBGM(const SoundKey& key);

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