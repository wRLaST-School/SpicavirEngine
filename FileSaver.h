#pragma once
#include <Essentials.h>
class FileSaver
{
public:
	static void Save();
	static void Overwrite();
	static void Open();

	static wstring filePath; 
};
