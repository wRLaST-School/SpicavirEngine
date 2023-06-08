#include "Util.h"

std::wstring Util::StrToWStr(std::string str, int32_t page)
{
    // SJIS -> wstring
    int32_t iBufferSize = MultiByteToWideChar(page, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

    // �o�b�t�@�̎擾
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS -> wstring
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, cpUCS2, iBufferSize);

    // string�̐���
    std::wstring ret(cpUCS2, cpUCS2 + iBufferSize - 1);

    // �o�b�t�@�̔j��
    delete[] cpUCS2;

    // �ϊ����ʂ�Ԃ�
    return ret;
}

bool Util::Chance(int32_t percentage)
{
    return (Util::RNG(0, 99, true) < percentage);
}

int32_t Util::RNG(int32_t min, int32_t max, bool preciseMode)
{
    if (!preciseMode) {
        return (rand() % (max + 1 - min) + min);
    }

    int32_t ret = 0;
    do {
        ret = rand();
    } while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
    ret = ret % (max + 1 - min) + min;
    return ret;
}