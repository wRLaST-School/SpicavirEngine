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
	int resolution = 64;

	int weight = 1000;
	int charSet = SHIFTJIS_CHARSET;

	//�g�p����t�H���g�̖��O
	string name = "�l�r �o����";

	int gradFlag = GGO_GRAY8_BITMAP;
};

struct StringOptions
{
	FontOptions fontOptions;

	//�t�H���g�T�C�Y(pt)
	int size = 64;

	//�s��(px)
	int lineSpacing = 2;
	//����(px)
	int charSpacing = 2;
};

struct StringData
{
	TextureKey key;
	int width = 0;
	int height = 0;
};

class FontData
{
public:
	GLYPHMETRICS gm = {};
	TEXTMETRIC tm;
	vector<BYTE> bmp;
	int resolution;
	int grad = 0;
};

class TextDrawer
{
private:

public:
	static void DrawString(string str, int x, int y, Align alignment, StringOptions options);
	static void DrawString(string str, int x, int y, Align alignment);
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
typedef unsigned int FontNameHandle;

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

	int fontIndex = 0x8001;
};