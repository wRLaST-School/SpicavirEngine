#include "ExprAST.h"
#include <Windows.h>
#include <string>
#include <format>

namespace Libra {
    std::unique_ptr<ExprAST> Libra::Error(const char* str)
    {
        OutputDebugStringA(std::format("Error: {}\n", str).c_str());
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> Libra::ErrorP(const char* str)
    {
        Error(str);
        return nullptr;
    }

    std::unique_ptr<FunctionAST> Libra::ErrorF(const char* str)
    {
        Error(str);
        return nullptr;
    }
}
