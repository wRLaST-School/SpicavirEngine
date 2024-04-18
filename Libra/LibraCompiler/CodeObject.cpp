#include "CodeObject.h"
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <format>

void Libra::CodeObject::LoadCodes(std::string filePath)
{
    fullCode.clear();
 
    std::ifstream file;
    std::stringstream sstream;

    file.open(filePath.c_str());

    if (!file) {
        fullCode = "No File";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        sstream << line << "\n";
    }

    fullCode = sstream.str();

    file.close();
}

void Libra::CodeObject::OutputLLVMIRCode(std::string filePath)
{
    std::ofstream file;

    std::string output;

    llvm::raw_string_ostream os(output);

    TheModule->print(os, nullptr);

    file.open(filePath);

    file << output;

    file.close();
}

char Libra::CodeObject::GetNextChar()
{
    if (curIndex >= fullCode.length())
    {
        return EOF;
    }

    char r = fullCode[curIndex];
    curIndex++;

    return r;
}
