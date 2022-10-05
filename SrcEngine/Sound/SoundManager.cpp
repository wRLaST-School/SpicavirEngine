#include "SoundManager.h"

SoundManager* SoundManager::GetInstance()
{
    SoundManager obj;
    return &obj;
}

void SoundManager::Init()
{
    XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    xAudio2->CreateMasteringVoice(&masterVoice);
    sndMap.clear();
}

SoundKey SoundManager::LoadWave(string path, SoundKey key)
{
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

    sndMap.emplace(key, soundData);

    return key;
}

void SoundManager::Play(SoundKey key)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;//‚±‚ê•Û‘¶‚µ‚Æ‚­‚ÆŽ~‚ß‚ç‚ê‚é
    SoundData* pSnd = &sndMap[key];

    xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

    XAUDIO2_BUFFER buf{};

    buf.pAudioData = pSnd->pBuffer;
    buf.AudioBytes = pSnd->bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    pSourceVoice->SubmitSourceBuffer(&buf);
    pSourceVoice->Start();
}
SoundData* SoundManager::PlayBGM(SoundKey key, bool loopFlag)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;//‚±‚ê•Û‘¶‚µ‚Æ‚­‚ÆŽ~‚ß‚ç‚ê‚é
    SoundData* pSnd = &sndMap[key];

    if (pSnd->sound != nullptr)
    {
        pSnd->sound->Stop();
    }

    xAudio2->CreateSourceVoice(&pSourceVoice, &pSnd->wfex);

    XAUDIO2_BUFFER buf{};

    buf.pAudioData = pSnd->pBuffer;
    buf.AudioBytes = pSnd->bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;
    if(loopFlag) buf.LoopCount = XAUDIO2_LOOP_INFINITE;

    pSourceVoice->SubmitSourceBuffer(&buf);
    pSourceVoice->Start();

    pSnd->sound = pSourceVoice;

    return pSnd;
}

void SoundManager::StopBGM(SoundKey key)
{
    SoundData* pSnd = &sndMap[key];
    pSnd->sound->Stop();
}

void SoundManager::ReleaseAllSounds()
{
    for (auto itr = sndMap.begin(); itr != sndMap.end(); itr++)
    {
        itr->second.Release();
    }
    sndMap.clear();
}

ComPtr<IXAudio2> SoundManager::xAudio2;
IXAudio2MasteringVoice* SoundManager::masterVoice;
map<SoundKey, SoundData> SoundManager::sndMap;