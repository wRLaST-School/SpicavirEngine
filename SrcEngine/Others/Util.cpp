#include "Util.h"
#include <random>
#include <GameManager.h>
#include <SpWindow.h>

std::random_device rnd;
std::mt19937 mt(rnd());

std::wstring Util::StrToWStr(const std::string& str, uint32_t page)
{
    // SJIS -> wstring
    int32_t iBufferSize = MultiByteToWideChar(page, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS -> wstring
    MultiByteToWideChar(page, 0, str.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring ret(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return ret;
}

Float2 Util::GetWinSize()
{
    if (GameManager::sShowDebug)
    {

    }
    return Float2(static_cast<float>(GetSpWindow()->width), 
        static_cast<float>(GetSpWindow()->height));
}

int32_t Util::GetWinWidth()
{
    return static_cast<int32_t>(GetWinSize().x);
}

int32_t Util::GetWinHeight()
{
    return static_cast<int32_t>(GetWinSize().y);
}

bool Util::Chance(int32_t percentage)
{
    return (Util::RNG(0, 99, true) < percentage);
}

int32_t Util::RNG(int32_t min, int32_t max, bool preciseMode)
{
    if (!preciseMode) {
        return (mt() % (max + 1 - min) + min);
    }

    std::uniform_int_distribution<> rbrnd(min, max);

    return rbrnd(mt);
}