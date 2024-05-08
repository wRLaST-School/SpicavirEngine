#include "LibraCompiler.h"
#include <Windows.h>
//#include "Parser.h"
#include "CodeObject.h"

void Libra::Compiler::Compile(std::string scriptPath, std::string dllPath)
{
	//コンパイル処理を書く
	//ExprAST用のLLVMライブラリの初期化
	//Libra::InitLLVM();

	//CodeObject::LoadCodes(scriptPath);

	//Parser parser;

	//parser.ParseCodeLoop();

	//CodeObject::OutputLLVMIRCode((scriptPath + ".ll"));
}

void Libra::Compiler::TestCPPCompile()
{
	//LibraTestDLLのコンパイル
	Sleep(30000);
}
