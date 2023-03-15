#include "stdafx.h"
#include "FileSaver.h"
#include <SpWindow.h>
#include <ShObjIdl.h> 
#include <fstream>
#include <sstream>
#include <MapChip.h>
#include <SnakeHead.h>
#include <SnakeBody.h>

wstring FileSaver::filePath = L"";

// 読み込んだファイルを区切り文字で分割する関数
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
        ofn.lpstrInitialDir = filePath == L"" ? szFolder : filePath.c_str();       // 初期フォルダ位置
        ofn.lpstrFile = szFile;       // 選択ファイル格納
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = L".csv";
        ofn.lpstrFilter = TEXT("マップマスタファイル(*.sps)\0*.sps\0")
            TEXT("すべてのファイル(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("開く");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }

    GetSaveFileNameW(&ofn);

    if (szFile[0] == '\0')
    {
        return;
    };

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

	//セーブ処理
	OutputDebugStringA("保存が呼び出されました\n");

    //filePathに最後に開いた情報があるんでそれ使っていい感じに保存
    ofstream file;
    file.open(filePath, ios::out);
    file.close();

    SaveMapChip(filePath);
    SaveSnakeHead(filePath);
    SaveSnakeBody(filePath);
}

void FileSaver::Overwrite()
{
    //上書き処理
    OutputDebugStringA("上書き保存が呼び出されました\n");

    //filePathが空なら新規保存の関数に移行
    if (filePath == L"")
    {
        return Save();
    }
    //filePathに最後に開いた情報があるんでそれ使っていい感じに保存
    ofstream file;
    file.open(filePath, ios::out);
    file.close();

    SaveMapChip(filePath);
    SaveSnakeHead(filePath);
    SaveSnakeBody(filePath);
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
        ofn.lpstrInitialDir = filePath == L"" ? szFolder : filePath.c_str();       // 初期フォルダ位置
        ofn.lpstrFile = szFile;       // 選択ファイル格納
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT("マップマスタファイル(*.sps)\0*.sps\0")
                            TEXT("すべてのファイル(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("開く");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    }

    GetOpenFileNameW(&ofn);

    if (szFile[0] == '\0')
    {
        return;
    };

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

    //filePathにパスが入ってるのでいい感じに読み込む
    LoadMapChip(filePath);
    LoadSnakeHead(filePath);
    LoadSnakeBody(filePath);
}

void FileSaver::ResetPath()
{
    filePath = L"";
}

void FileSaver::SaveMapChip(wstring filePath)
{
    ofstream file;
    file.open(filePath + wstring(L"00.stg"), ios::out);
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

void FileSaver::SaveSnakeHead(wstring filePath)
{
    ofstream file;
    file.open(filePath + wstring(L"10.stg"), ios::out);
    for (int y = 0; y < SnakeHead::heads.size(); y++)
    {
        file << SnakeHead::heads.at(y).x << "," << SnakeHead::heads.at(y).y << "," << SnakeHead::heads.at(y).id << "," << endl;
    }

    file.close();
}

void FileSaver::SaveSnakeBody(wstring filePath)
{
    ofstream file;
    file.open(filePath + wstring(L"11.stg"), ios::out);
    for (int y = 0; y < SnakeBody::bodies.size(); y++)
    {
        file << SnakeBody::bodies.at(y).x << "," << SnakeBody::bodies.at(y).y << "," << SnakeBody::bodies.at(y).id << "," << SnakeBody::bodies.at(y).order << "," << endl;
    }

    file.close();
}

void FileSaver::LoadMapChip(wstring filePath)
{
    std::ifstream ifs(filePath + wstring(L"00.stg"));	// 読み込み用のストリーム

    if (ifs)
    {
        std::string line;

        int lines = 0;

        MapChip::Init(0, 0);

        while (getline(ifs, line))
        {
            std::vector<int> datvec;
            std::vector<std::string> strvec = split(line, ',');
            for (auto&& s : strvec) datvec.push_back(std::stoi(s));	// セルの文字列を数値に変換
            MapChip::map.push_back(datvec);
            MapChip::mapSizeX = max(MapChip::map.back().size(), MapChip::mapSizeX);

            lines++;
        }
        MapChip::mapSizeY = lines;
    }
}

void FileSaver::LoadSnakeHead(wstring filePath)
{
    std::ifstream ifs(filePath + wstring(L"10.stg"));	// 読み込み用のストリーム

    if (ifs)
    {
        std::string line;

        SnakeHead::Init();

        while (getline(ifs, line))
        {
            std::vector<int> datvec;
            std::vector<std::string> strvec = split(line, ',');
            SnakeHead::heads.emplace_back(
                stoi(strvec.at(0)),
                stoi(strvec.at(1)),
                stoi(strvec.at(2)));
        }
    }
}

void FileSaver::LoadSnakeBody(wstring filePath)
{
    std::ifstream ifs(filePath + wstring(L"11.stg"));	// 読み込み用のストリーム

    if (ifs)
    {
        std::string line;

        SnakeBody::Init();

        while (getline(ifs, line))
        {
            std::vector<int> datvec;
            std::vector<std::string> strvec = split(line, ',');
            SnakeBody::bodies.emplace_back(
                stoi(strvec.at(0)),
                stoi(strvec.at(1)),
                stoi(strvec.at(2)),
                stoi(strvec.at(3)));
        }
    }
}
