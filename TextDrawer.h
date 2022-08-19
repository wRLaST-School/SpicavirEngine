#pragma once
#include "Sprite.h"
#include "wTextureManager.h"

enum class Alignment
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	TopCenter,
	BottomCenter,
	CenterLeft,
	CenterRight,
	Center
};

struct FontOptions
{
	//フォントの解像度、シーンごとに統一することを推奨
	int resolution = 64;

	int weight = 1000;
	int charSet = SHIFTJIS_CHARSET;

	//使用するフォントの名前
	string name = "ＭＳ Ｐ明朝";

	int gradFlag = GGO_GRAY8_BITMAP;
};

struct StringOptions
{
	FontOptions fontOptions;

	//フォントサイズ(pt)
	int size = 32;

	//行間(px)
	int lineSpacing = 2;
	//字間(px)
	int charSpacing = 2;
};

struct StringData
{
	TextureKey key;
	int width;
	int height;
};

class FontData
{
public:
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	vector<BYTE> bmp;
	int resolution;
	int grad;
};

class TextDrawer
{
private:

public:

	static void DrawString(string str, int x, int y, Alignment alignment, StringOptions options);
	static void DrawString(string str, int x, int y, Alignment alignment);
	static void SetDefaultStringOptions(StringOptions options);
	static StringOptions GetDefaultStringOptions();
private:
	vector<TextureKey> releaseQueue;
};

typedef FontData* FontHandle;
typedef unsigned int FontNameHandle;

class FontManager
{
public:
	static FontHandle GetGlyphTexture(FontOptions options, wstring glyph);

	static StringData CreateStringTexture(string str, StringOptions options);

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