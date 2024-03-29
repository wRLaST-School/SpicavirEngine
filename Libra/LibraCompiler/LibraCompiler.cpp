#include "LibraCompiler.h"
#include <Windows.h>
#include "Parser.h"

void Libra::Compiler::Compile(std::string scriptPath, std::string dllPath)
{
	//コンパイル処理を書く
	//ExprAST用のLLVMライブラリの初期化
	Libra::InitLLVM();
}

void Libra::Compiler::TestCPPCompile()
{
	//LibraTestDLLのコンパイル
	Sleep(30000);
}
