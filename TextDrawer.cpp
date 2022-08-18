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

	return FontHandle();
}
