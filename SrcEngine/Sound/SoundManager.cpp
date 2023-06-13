#include "SoundManager.h"

SoundManager* SoundManager::GetInstance()
{
    static SoundManager obj;
    return &obj;
}

void SoundManager::Init()
{
    XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    xAudio2->CreateMasteringVoice(&masterVoice);
    sndMap.clear();
}

SoundKey SoundManager::LoadWave(const string& path, const SoundKey& key)
{
    sndMap.Access(
        [&](auto& map) {
            if (map.count(key) <= 0) { return; }
        }
    );
    ifstream file;

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

    sndMap.Access(
        [&](auto& map) {
            map.emplace(key, soundData);
        }
    );
    perSceneSounds[currentSceneResIndex].push_back(key);

    return key;
}

void SoundManager::Play(const SoundKey& key)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる

    SoundData* pSnd;
    sndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );

    xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

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
    sndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );

    if (pSnd->sound != nullptr)
    {
        pSnd->sound->Stop();
    }

    xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

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
    sndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );
    return pSnd;
}

void SoundManager::StopBGM(const SoundKey& key)
{
    SoundData* pSnd;
    sndMap.Access(
        [&](auto& map) {
            pSnd = &map[key];
        }
    );
    if (pSnd->sound == nullptr) return;
    pSnd->sound->Stop();
}

void SoundManager::ReleaseAllSounds()
{
    sndMap.Access(
        [&](auto& map) {
            for (auto itr = map.begin(); itr != map.end(); itr++)
            {
                itr->second.Release();
            }
        }
    );
    sndMap.clear();
}

void SoundManager::ReleasePerSceneSounds()
{
    int32_t lastSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
    for (auto itr = perSceneSounds[lastSceneResIndex].begin(); itr != perSceneSounds[lastSceneResIndex].end(); itr++)
    {
        bool usingInCurrentScene = false;
        for (auto key : perSceneSounds[currentSceneResIndex])
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
            sndMap.Access(
                [&](auto& map) {
                    map.erase(*itr);
                }
            );
        }
    }

    perSceneSounds[lastSceneResIndex].clear();
}

void SoundManager::PreLoadNewScene()
{
    currentSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
}

ComPtr<IXAudio2> SoundManager::xAudio2;
IXAudio2MasteringVoice* SoundManager::masterVoice;
exc_unordered_map<SoundKey, SoundData> SoundManager::sndMap;
list<SoundKey> SoundManager::perSceneSounds[2];
int32_t SoundManager::currentSceneResIndex;
