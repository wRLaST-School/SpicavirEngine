#pragma once
#include <Essentials.h>
class FileSaver
{
public:
	static void Save();
	static void Overwrite();
	static void Open();
	static void ResetPath();

	static void SaveMapChip(wstring filePath);
	static void SaveSnakeHead(wstring filePath);
	static void SaveSnakeBody(wstring filePath);
	static void SaveMoveBlock(wstring filePath);

	static void LoadMapChip(wstring filePath);
	static void LoadSnakeHead(wstring filePath);
	static void LoadSnakeBody(wstring filePath);
	static void LoadMoveBlock(wstring filePath);

	static wstring filePath; 
};

