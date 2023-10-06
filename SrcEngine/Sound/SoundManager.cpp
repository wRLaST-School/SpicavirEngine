#include "SoundManager.h"

using namespace std;

SoundManager* SoundManager::GetInstance()
{
    static SoundManager obj;
    return &obj;
}

void SoundManager::Init()
{
    XAudio2Create(&sXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    sXAudio2->CreateMasteringVoice(&sMasterVoice);
    sSndMap.clear();
}

SoundKey SoundManager::LoadWave(const std::string& path, const SoundKey& key)
{
    bool isAlreadyRegistered = false;
    sSndMap.Access(
        [&](auto& map) {
            if (map.count(key) > 0) { isAlreadyRegistered = true; }
        }
    );

    if (isAlreadyRegistered)
    {
        sPerSceneSounds[sCurrentSceneResIndex].push_back(key);
        return key;
    }

    std::ifstream file;

    file.open(path, std::ios_base::binary);

    assert(file.is_open());

    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));

    if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
    {
        assert(0);
    }

    if (strncmp(riff.type, "WAVE", 4) != 0)
    {
        assert(0);
    }

    FormatChunk format = {};

    file.read((char*)&format, sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0)
    {
        assert(0);
    }

    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);

    ChunkHeader data;
    file.read((char*)&data, sizeof(data));

    if (strncmp(data.id, "JUNK", 4) == 0)
    {
        file.seekg(data.size, std::ios_base::cur);

        file.read((char*)&data, sizeof(data));
    }

    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }

    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);

    file.close();

    SoundData soundData = {};

    soundData.wfex = format.fmt;
    soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;

    sSndMap.Access(
        [&](auto& map) {
            map.emplace(key, soundData);
        }
    );
    sPerSceneSounds[sCurrentSceneResIndex].push_back(key);

    return key;
}

void SoundManager::Play(const SoundKey& key)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる

    SoundData* pSnd;
    sSndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );

    sXAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

    XAUDIO2_BUFFER buf{};

    buf.pAudioData = pSnd->pBuffer;
    buf.AudioBytes = pSnd->bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    pSourceVoice->SubmitSourceBuffer(&buf);
    pSourceVoice->Start();
    pSnd->sound = pSourceVoice;
}
SoundData* SoundManager::PlayBGM(const SoundKey& key, bool loopFlag)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる
    SoundData* pSnd;
    sSndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );

    if (pSnd->sound != nullptr)
    {
        pSnd->sound->Stop();
    }

    sXAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

    XAUDIO2_BUFFER buf{};

    buf.pAudioData = pSnd->pBuffer;
    buf.AudioBytes = pSnd->bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;
    if (loopFlag) buf.LoopCount = XAUDIO2_LOOP_INFINITE;

    pSourceVoice->SubmitSourceBuffer(&buf);
    pSourceVoice->Start();

    pSnd->sound = pSourceVoice;

    return pSnd;
}

SoundData* SoundManager::GetSoundData(const SoundKey& key)
{
    SoundData* pSnd;
    sSndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );
    return pSnd;
}

void SoundManager::StopBGM(const SoundKey& key)
{
    SoundData* pSnd;
    sSndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );
    if (pSnd->sound == nullptr) return;

    pSnd->sound->Stop();
}

void SoundManager::ReleaseAllSounds()
{
    sSndMap.Access(
        [&](auto& map) {
            for (auto itr = map.begin(); itr != map.end(); itr++)
            {
                itr->second.Release();
            }
        }
    );
    sSndMap.clear();
}

void SoundManager::ReleasePerSceneSounds()
{
    int32_t lastSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
    for (auto itr = sPerSceneSounds[lastSceneResIndex].begin(); itr != sPerSceneSounds[lastSceneResIndex].end(); itr++)
    {
        bool usingInCurrentScene = false;
        for (auto& key : sPerSceneSounds[sCurrentSceneResIndex])
        {
            if (key == *itr)
            {
                usingInCurrentScene = true;
            }
        }

        if (!usingInCurrentScene) //今のシーンで使われていないならリリース
        {
            StopBGM(*itr);
            GetSoundData(*itr)->Release();
            sSndMap.Access(
                [&](auto& map) {
                    map.erase(*itr);
                }
            );
        }
    }

    sPerSceneSounds[lastSceneResIndex].clear();
}

void SoundManager::PreLoadNewScene()
{
    sCurrentSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
}

Microsoft::WRL::ComPtr<IXAudio2> SoundManager::sXAudio2;
IXAudio2MasteringVoice* SoundManager::sMasterVoice;
exc_unordered_map<SoundKey, SoundData> SoundManager::sSndMap;
list<SoundKey> SoundManager::sPerSceneSounds[2];
int32_t SoundManager::sCurrentSceneResIndex;
