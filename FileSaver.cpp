#include "stdafx.h"
#include "FileSaver.h"
#include <SpWindow.h>
#include <ShObjIdl.h> 
#include <fstream>
#include <sstream>
#include <MapChip.h>

wstring FileSaver::filePath = L"";

// �ǂݍ��񂾃t�@�C������؂蕶���ŕ�������֐�
std::vector<std::string> split(std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::string field;
    std::vector<std::string> result;
    while (getline(stream, field, delimiter)) result.push_back(field);
    return result;
}

void FileSaver::Save()
{
    OPENFILENAME     ofn{};
    TCHAR            szFolder[MAX_PATH]{};
    TCHAR            szFile[MAX_PATH]{};

    if (filePath == L"") {
        GetCurrentDirectory(MAX_PATH, szFolder);
    }

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetSpWindow()->hwnd;
        ofn.lpstrInitialDir = filePath == L"" ? szFolder : filePath.c_str();       // �����t�H���_�ʒu
        ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = L".csv";
        ofn.lpstrFilter = TEXT("�}�b�v�`�b�v�t�@�C��(*.csv)\0*.csv\0")
            TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("�J��");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }

    GetSaveFileNameW(&ofn);

    if (szFile[0] == '\0')
    {
        return;
    };

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

	//�Z�[�u����
	OutputDebugStringA("�ۑ����Ăяo����܂���\n");

    //filePath�ɍŌ�ɊJ������񂪂����ł���g���Ă��������ɕۑ�
    ofstream file;
    file.open(filePath, ios::out);
    for (int y = 0; y < MapChip::mapSizeY; y++)
    {
        for (int x = 0; x < MapChip::mapSizeX; x++)
        {
            file << MapChip::map.at(y).at(x) << ",";
        }
        file << endl;
    }

    file.close();
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

    ofstream file;
    file.open(filePath, ios::out);
    for (int y = 0; y < MapChip::mapSizeY; y++)
    {
        for (int x = 0; x < MapChip::mapSizeX; x++)
        {
            file << MapChip::map.at(y).at(x) << ",";
        }
        file << endl;
    }

    file.close();
}

void FileSaver::Open()
{
    OPENFILENAME     ofn{};
    TCHAR            szFolder[MAX_PATH]{};
    TCHAR            szFile[MAX_PATH]{};

    if (filePath == L"") {
        GetCurrentDirectory(MAX_PATH, szFolder);
    }

    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetSpWindow()->hwnd;
        ofn.lpstrInitialDir = filePath == L"" ? szFolder : filePath.c_str();       // �����t�H���_�ʒu
        ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT("�}�b�v�`�b�v�t�@�C��(*.csv)\0*.csv\0")
                            TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("�J��");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }

    GetOpenFileNameW(&ofn);

    if (szFile[0] == '\0')
    {
        return;
    };

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

    //filePath�Ƀp�X�������Ă�̂ł��������ɓǂݍ���

    std::ifstream ifs(filePath);	// �ǂݍ��ݗp�̃X�g���[��

    if (ifs)
    {
        std::string line;

        int lines = 0;

        MapChip::Init(0, 0);

        while (getline(ifs, line))
        {
            std::vector<int> datvec;
            std::vector<std::string> strvec = split(line, ',');
            for (auto&& s : strvec) datvec.push_back(std::stoi(s));	// �Z���̕�����𐔒l�ɕϊ�
            MapChip::map.push_back(datvec);
            MapChip::mapSizeX = max(MapChip::map.back().size(), MapChip::mapSizeX);

            lines++;
        }
        MapChip::mapSizeY = lines;
    }
}

void FileSaver::ResetPath()
{
    filePath = L"";
}
