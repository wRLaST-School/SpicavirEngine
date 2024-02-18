#include "ExprAST.h"
#include <Windows.h>
#include <string>
#include <format>

namespace Libra {
    ExprAST* Libra::Error(const char* str)
    {
        OutputDebugStringA(std::format("Error: {}\n", str).c_str());
        return nullptr;
    }

    PrototypeAST* Libra::ErrorP(const char* str)
    {
        Error(str);
        return nullptr;
    }

    FunctionAST* Libra::ErrorF(const char* str)
    {
        Error(str);
        return nullptr;
    }
}
