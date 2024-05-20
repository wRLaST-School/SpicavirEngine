#include "CPPCompiler.h"
#include <string>
#include <fstream>
#include <sstream>

void CPPCompiler::Compile()
{
	//ビルドコマンドを生成していく
	std::stringstream cmd;

	//MSBuildフォルダの指定
	std::ifstream msbuildDirFile("Resources/CompilerData/MSBuildDirectory.txt");

	std::string msbuildDir;

	if (!msbuildDirFile)
	{
		msbuildDir = "";
	}
	else
	{
		std::getline(msbuildDirFile, msbuildDir);
	}

	msbuildDirFile.close();

	//MSBuildコマンドの実行
	cmd << msbuildDir;
	cmd << "MSBuild.exe ";

	//コンパイルするプロジェクトの指定
	cmd << "LibraTestProj/LibraTestDLL/LibraTestDLL.vcxproj ";

	//オプションの選択
	cmd << "/t:build "; //インクリメンタルビルドを実行

	cmd << "/p:Configuration=";

	//デバッグかリリースか
#ifdef _DEBUG
	cmd << "Debug";
#else
	cmd << "Release";
#endif
	cmd << ";" << "Platform=\"x64\" ";

	//ログ出力
	cmd << "/v:diag /fl";

	//デバッグ一時停止
	cmd << " & pause";

	std::string buf;
	buf = cmd.str();

	system(cmd.str().c_str());
}
