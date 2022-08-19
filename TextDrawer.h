#pragma once
#include "Sprite.h"
#include "wTextureManager.h"
class TextDrawer
{
private:
	static const int maxChar = 256;
	static const int fontWidth = 64;
	static const int fontHeight = 128;
	static const int charsPerLine = 14;

public:
	Sprite sprites[maxChar];
	int nextSpriteIndex = 0;

	static void Init();

private:
};

class FontOptions
{
public:
	int size = 64;
	int weight = 1000;
	int charSet = SHIFTJIS_CHARSET;
	string name;

	int gradFlag = GGO_GRAY8_BITMAP;
};

class FontData
{
public:
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
};

typedef FontData* FontHandle;
typedef unsigned int FontNameHandle;

class FontManager
{
private:
	using FPTR = unique_ptr<FontData>;

public:
	static FontHandle GetGlyphTexture(FontOptions options, wstring glyph);

	static TextureKey CreateStringTexture(string str, FontOptions options);

	///<summary>フォントをファイルからWindowsに読み込ませる</summary>
	///<param name = "path">読み込むファイルのexeからの相対パス / フルパス</param>
	///<returns>読み込んだフォントを使用するための名前</returns>
	static string LoadFontFromFile(string path);

	static FontManager* GetInstance();

private:
	map<FontNameHandle, map<wstring, FontData>> fontMap;
	map<string, FontNameHandle> fontNameMap;

	FontData* GetFontData(string fontName, wstring glyph);

	int fontIndex = 0x8001;
};