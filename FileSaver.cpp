#include "stdafx.h"
#include "FileSaver.h"
#include <SpWindow.h>
#include <ShObjIdl.h> 

wstring FileSaver::filePath = L"";

void FileSaver::Save()
{
    OPENFILENAME     ofn{};
    TCHAR            szPath[MAX_PATH]{};
    TCHAR            szFile[MAX_PATH]{};

    if (szPath[0] == TEXT('\0')) {
        GetCurrentDirectory(MAX_PATH, szPath);
    }

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetSpWindow()->hwnd;
        ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
        ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = L".kasumaster";
        ofn.lpstrFilter = TEXT("�X�e�[�W�}�X�^�t�@�C��(*.kasumaster)\0*.kasumaster\0")
            TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("�J��");
        ofn.Flags = OFN_FILEMUSTEXIST;
    }

    GetSaveFileNameW(&ofn);

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

	//�Z�[�u����
	OutputDebugStringA("�ۑ����Ăяo����܂���\n");

    //filePath�ɍŌ�ɊJ������񂪂����ł���g���Ă��������ɕۑ�
}

void FileSaver::Overwrite()
{
    //�㏑������
    OutputDebugStringA("�㏑���ۑ����Ăяo����܂���\n");

    //filePath����Ȃ�V�K�ۑ��̊֐��Ɉڍs
    if (filePath == L"")
    {
        return Save();
    }
    //filePath�ɍŌ�ɊJ������񂪂����ł���g���Ă��������ɕۑ�
}

void FileSaver::Open()
{
    OPENFILENAME     ofn{};
    TCHAR            szPath[MAX_PATH]{};
    TCHAR            szFile[MAX_PATH]{};

    if (szPath[0] == TEXT('\0')) {
        GetCurrentDirectory(MAX_PATH, szPath);
    }

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetSpWindow()->hwnd;
        ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
        ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT("�X�e�[�W�}�X�^�t�@�C��(*.kasumaster)\0*.kasumaster\0")
                            TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("�J��");
        ofn.Flags = OFN_FILEMUSTEXIST;
    }

    GetOpenFileNameW(&ofn);

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

    //FilePath�Ƀp�X�������Ă�̂ł��������ɓǂݍ���
}
