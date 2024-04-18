#pragma once
#include<string>
#include <map>
#include "LLVMIncludes.h"

namespace Libra {
	class CodeObject
	{
	public:
		static void LoadCodes(std::string filePath);

		static void OutputLLVMIRCode(std::string filePath);

		static char GetNextChar();

		inline static std::string fullCode;
		inline static size_t curIndex;


		inline static std::unique_ptr<llvm::LLVMContext> TheContext;
		inline static std::unique_ptr<llvm::IRBuilder<>> Builder;
		inline static std::unique_ptr<llvm::Module> TheModule;
		inline static std::map<std::string, llvm::Value*> NamedValues;
	};
}

