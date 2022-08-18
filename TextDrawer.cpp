#include "TextDrawer.h"
#include "wDirectX.h"
#include "Util.h"

void TextDrawer::Init()
{ 
	
}

FontHandle FontManager::GetGlyphTexture(FontOptions options, wstring glyph)
{
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
	TEXTMETRIC tm;

	GetTextMetrics(hdc, &tm);

	GLYPHMETRICS gm;

	CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	BYTE* pMono = new BYTE[options.size];

	DWORD size = GetGlyphOutlineW(hdc, code, options.gradFlag, &gm, options.size, pMono, &mat);

	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	

	return FontHandle();
}

FontData* FontManager::GetFontData(string fontName, string glyph)
{
	FontNameHandle handle;
	auto itr = fontNameMap.find(fontName);
	if ( itr == fontNameMap.end()) return nullptr;
	handle = itr->second;
	return &fontMap[handle][glyph];
}
