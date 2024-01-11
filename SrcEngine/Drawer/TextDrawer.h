#pragma once
#include "Sprite.h"
#include "SpTextureManager.h"

enum class Align
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
	int32_t resolution = 64;

	int32_t weight = 1000;
	int32_t charSet = SHIFTJIS_CHARSET;

	//使用するフォントの名前
	std::string name = "ＭＳ Ｐ明朝";

	int32_t gradFlag = GGO_GRAY8_BITMAP;
};

struct StringOptions
{
	FontOptions fontOptions;

	//フォントサイズ(pt)
	int32_t size = 64;

	//行間(px)
	int32_t lineSpacing = 2;
	//字間(px)
	int32_t charSpacing = 2;
};

struct StringData
{
	TextureKey key;
	int32_t width = 0;
	int32_t height = 0;
};

class FontData
{
public:
	GLYPHMETRICS gm = {};
	TEXTMETRIC tm;
	std::vector<BYTE> bmp;
	int32_t resolution;
	int32_t grad = 0;
};

struct StringSpDSData {
	TextureKey key;
	Float2 scale = {};
	Vec2 pos = {};
};

class TextDrawer
{
private:

public:
	static void DrawString(std::string str, int32_t x, int32_t y, Align alignment, StringOptions options);
	static void DrawString(std::string str, int32_t x, int32_t y, Align alignment);
	static void SetDefaultStringOptions(StringOptions options);

	static void ReleaseDrawStringData();
	static StringOptions GetDefaultStringOptions();

	static TextDrawer* GetInstance();
private:
	std::vector<TextureKey> releaseQueue;
	std::vector<StringSpDSData> stringSpriteQueue;
	StringOptions defaultOption = StringOptions();
};

typedef FontData* FontHandle;
typedef uint32_t FontNameHandle;

class FontManager
{
public:
	static FontHandle GetGlyphTexture(FontOptions options, std::wstring glyph);

	static StringData CreateStringTexture(std::string str, StringOptions options);

	///<summary>フォントをファイルからWindowsに読み込ませる</summary>
	///<param name = "path">読み込むファイルのexeからの相対パス / フルパス</param>
	///<returns>読み込んだフォントを使用するための名前</returns>
	static std::string LoadFontFromFile(std::string path);

	static FontManager* GetInstance();

private:
	std::map<FontNameHandle, std::map<std::wstring, FontData>> fontMap;
	std::map<std::string, FontNameHandle> fontNameMap;

	FontData* GetFontData(std::string fontName, std::wstring glyph);

	int32_t fontIndex = 0x8001;
};