#pragma once
#include<string>

namespace Libra {
	class Compiler
	{
	public:
		static void Compile(std::string scriptPath, std::string dllPath);

		static void TestCPPCompile();
	};
}

