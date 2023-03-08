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
        ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
        ofn.lpstrFile = szFile;       // 選択ファイル格納
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = L".kasumaster";
        ofn.lpstrFilter = TEXT("ステージマスタファイル(*.kasumaster)\0*.kasumaster\0")
            TEXT("すべてのファイル(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("開く");
        ofn.Flags = OFN_FILEMUSTEXIST;
    }

    GetSaveFileNameW(&ofn);

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

	//セーブ処理
	OutputDebugStringA("保存が呼び出されました\n");

    //filePathに最後に開いた情報があるんでそれ使っていい感じに保存
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
        ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
        ofn.lpstrFile = szFile;       // 選択ファイル格納
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = TEXT("ステージマスタファイル(*.kasumaster)\0*.kasumaster\0")
                            TEXT("すべてのファイル(*.*)\0*.*\0");
        ofn.lpstrTitle = TEXT("開く");
        ofn.Flags = OFN_FILEMUSTEXIST;
    }

    GetOpenFileNameW(&ofn);

    filePath = szFile;

    OutputDebugStringW(filePath.c_str());

    //FilePathにパスが入ってるのでいい感じに読み込む
}
