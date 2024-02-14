#include "stdafx.h"
#include "DLLObject.h"
#include <LibraTestProj/LibraTestDLL/TestPlayer.h>

const HMODULE& Libra::DLLObject::LoadDLL(const std::string& path)
{
    //何か登録されてたら解放
    if (hModule_) { Free(); }
    
    //DLL読み込み
    hModule_ = LoadLibraryA(path.c_str());

    if (hModule_ == nullptr) { return hModule_; }

    //Create関数でインスタンス生成するのでその関数ポインタを取得
    using fType = IScriptObject * (*)();
    fType instantiateFunc = reinterpret_cast<fType>(GetProcAddress(hModule_, "Create"));

    if (instantiateFunc == nullptr) { return hModule_; }

    //生成を行う
    component_ = instantiateFunc();

    return hModule_;
}

const HMODULE& Libra::DLLObject::GetModule()
{
    return hModule_;
}

IScriptObject* Libra::DLLObject::GetComponent()
{
    return component_;
}

Libra::DLLObject::~DLLObject()
{
    Free();
}

void Libra::DLLObject::Free()
{
    FreeLibrary(hModule_);
}
