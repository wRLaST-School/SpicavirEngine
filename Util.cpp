#include "Util.h"

std::wstring Util::StrToWStr(std::string str, int page)
{
    // SJIS -> wstring
    int iBufferSize = MultiByteToWideChar(page, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

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