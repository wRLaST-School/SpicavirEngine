#include "TextDrawer.h"
#include "wDirectX.h"
#include "Util.h"

void TextDrawer::Init()
{ 
	
}

FontHandle FontManager::GetGlyphTexture(FontOptions options, wstring glyph)
{
	FontManager* ins = GetInstance();

	//このフォントを呼ぶのが初めてならマップにフォントを追加
	FontNameHandle handle;
	map<wstring, FontData>* pFontDataMap = nullptr;
	auto itr = ins->fontNameMap.find(options.name);
	if (itr == ins->fontNameMap.end())
	{
		handle = ins->fontNameMap.emplace(options.name, ins->fontIndex).first->second;
		pFontDataMap = &(ins->fontMap.emplace(handle, map<wstring, FontData>()).first->second);
	}
	else
	{
		handle = itr->second;
		pFontDataMap = &ins->fontMap.find(handle)->second;
	}

	//このフォントでこの文字を読み込むのが初めてなら読み込み、そうでないならreturn
	auto itr2 = pFontDataMap->find(glyph);
	if (itr2 != pFontDataMap->end()) return &itr2->second;

	FontHandle pFont = &pFontDataMap->emplace(glyph, FontData()).first->second;

	LOGFONT lf =
	{
		options.size, 0, 0, 0, options.weight, 0,0,0,
		options.charSet, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH | FF_MODERN,
		*Util::StrToWStr(options.name).c_str()
	};

	HFONT hFont = CreateFontIndirectW(&lf);

	HDC hdc = GetDC(NULL);

	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	const wchar_t* c = glyph.c_str();
	UINT code = (UINT)*c;

	GetTextMetrics(hdc, &pFont->tm);

	CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	BYTE* pMono = new BYTE[options.size];

	DWORD size = GetGlyphOutlineW(hdc, code, options.gradFlag, &pFont->gm, options.size, pMono, &mat);

	return pFont;
}

FontManager* FontManager::GetInstance()
{
	static FontManager obj;
	return &obj;
}

FontData* FontManager::GetFontData(string fontName, wstring glyph)
{
	FontNameHandle handle;
	auto itr = fontNameMap.find(fontName);
	if ( itr == fontNameMap.end()) return nullptr;
	handle = itr->second;
	return &fontMap[handle][glyph];
}
