#include "stdafx.h"
#include "DLLObject.h"
#include <SpDS.h>

const HMODULE& Libra::DLLObject::LoadDLL(const std::string& className)
{
    //何か登録されてたら解放
    if (hModule_) { Free(); }
    
    //DLL読み込み
    hModule_ = LoadLibraryA("Resources/Compiled/Scripts.dll");

    if (hModule_ == nullptr) { return hModule_; }

    //Create関数でインスタンス生成するのでその関数ポインタを取得
    using fType = IScriptObject * (*)();

    //Create関数の関数名をクラス名から作成
    std::string createFuncName = className;
    createFuncName += "_Create";

    fType instantiateFunc = reinterpret_cast<fType>(GetProcAddress(hModule_, createFuncName.c_str()));

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
