#include "Util.h"

std::wstring Util::StrToWStr(std::string str, int page)
{
    // SJIS -> wstring
    int iBufferSize = MultiByteToWideChar(page, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS -> wstring
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring ret(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return ret;
}

bool Util::Chance(int percentage)
{
    return (Util::RNG(0, 99, true) < percentage);
}

int Util::RNG(int min, int max, bool preciseMode)
{
    if (!preciseMode) {
        return (rand() % (max + 1 - min) + min);
    }

    int ret = 0;
    do {
        ret = rand();
    } while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
    ret = ret % (max + 1 - min) + min;
    return ret;
}