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
	//�t�H���g�̉𑜓x�A�V�[�����Ƃɓ��ꂷ�邱�Ƃ𐄏�
	int32_t resolution = 64;

	int32_t weight = 1000;
	int32_t charSet = SHIFTJIS_CHARSET;

	//�g�p����t�H���g�̖��O
	string name = "�l�r �o����";

	int32_t gradFlag = GGO_GRAY8_BITMAP;
};

struct StringOptions
{
	FontOptions fontOptions;

	//�t�H���g�T�C�Y(pt)
	int32_t size = 64;

	//�s��(px)
	int32_t lineSpacing = 2;
	//����(px)
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
	vector<BYTE> bmp;
	int32_t resolution;
	int32_t grad = 0;
};

class TextDrawer
{
private:

public:
	static void DrawString(string str, int32_t x, int32_t y, Align alignment, StringOptions options);
	static void DrawString(string str, int32_t x, int32_t y, Align alignment);
	static void SetDefaultStringOptions(StringOptions options);

	static void ReleaseDrawStringData();
	static StringOptions GetDefaultStringOptions();

	static TextDrawer* GetInstance();
private:
	vector<TextureKey> releaseQueue;
	vector<Sprite> stringSpriteQueue;
	StringOptions defaultOption = StringOptions();
};

typedef FontData* FontHandle;
typedef uint32_t FontNameHandle;

class FontManager
{
public:
	static FontHandle GetGlyphTexture(FontOptions options, wstring glyph);

	static StringData CreateStringTexture(string str, StringOptions options);

	///<summary>�t�H���g���t�@�C������Windows�ɓǂݍ��܂���</summary>
	///<param name = "path">�ǂݍ��ރt�@�C����exe����̑��΃p�X / �t���p�X</param>
	///<returns>�ǂݍ��񂾃t�H���g���g�p���邽�߂̖��O</returns>
	static string LoadFontFromFile(string path);

	static FontManager* GetInstance();

private:
	map<FontNameHandle, map<wstring, FontData>> fontMap;
	map<string, FontNameHandle> fontNameMap;

	FontData* GetFontData(string fontName, wstring glyph);

	int32_t fontIndex = 0x8001;
};