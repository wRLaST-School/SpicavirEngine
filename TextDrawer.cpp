#include "TextDrawer.h"
#include "wDirectX.h"
#include "Util.h"
#include <chrono>

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

	//このフォントでこの文字を読み込むのが初めてか、既存の物より大きいフォントサイズなら読み込み、そうでないならreturn
	auto itr2 = pFontDataMap->find(glyph);
	FontHandle pFont = nullptr;
	if (itr2 != pFontDataMap->end())
	{
		if (itr2->second.resolution >= options.resolution)
			return &itr2->second;
		else
		{
			pFont = &itr2->second;
			pFont->bmp.clear();
		}
	}

	else 
	{
		pFont = &pFontDataMap->emplace(glyph, FontData()).first->second;
	}

	//LOGFONT lf =
	//{
	//	options.resolution, 0, 0, 0, options.weight, 0,0,0,
	//	options.charSet, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
	//	DEFAULT_PITCH | FF_MODERN,
	//	*Util::StrToWStr(options.name).c_str()
	//};

	wstring wfname = Util::StrToWStr(options.name);

	HFONT hFont = CreateFont(options.resolution, 0, 0, 0, options.weight, false, false, false, options.charSet,
			OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 0, wfname.c_str());

	HDC hdc = GetDC(NULL);

	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	const wchar_t* c = glyph.c_str();
	UINT code = (UINT)*c;

	GetTextMetrics(hdc, &pFont->tm);

	CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

	DWORD size = GetGlyphOutlineW(hdc, code, options.gradFlag, &pFont->gm, 0, NULL, &mat);

	BYTE* pMono = new BYTE[size];
	
	GetGlyphOutlineW(hdc, code, options.gradFlag, &pFont->gm, size, pMono, &mat);

	pFont->resolution = options.resolution;

	int bmpX = (pFont->gm.gmBlackBoxX + 3) / 4 * 4;
	BYTE* finalGlyph = new BYTE[pFont->gm.gmBlackBoxX * pFont->gm.gmBlackBoxY];

	//幅をBlackBoxに合わせる
	for (int i = 0; i < pFont->gm.gmBlackBoxX; i++)
	{
		for (int j = 0; j < pFont->gm.gmBlackBoxY; j++)
		{
			finalGlyph[j * pFont->gm.gmBlackBoxX + i] = pMono[j * bmpX + i];
		}
	}

	for (int i = 0; i < pFont->gm.gmBlackBoxX * pFont->gm.gmBlackBoxY; i++)
	{
		pFont->bmp.emplace_back(finalGlyph[i]);
	}

	switch (options.gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		pFont->grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		pFont->grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		pFont->grad = 64;
		break;
	}
	
	delete[] pMono;
	delete[] finalGlyph;

	if (pFont->bmp.size() != pFont->gm.gmBlackBoxX * pFont->gm.gmBlackBoxY)
	{
		throw "you are sussy baka";
		return nullptr;
	}

	return pFont;
}

StringData FontManager::CreateStringTexture(string str, StringOptions options)
{
	if (str == "") return StringData();

	wstring wStr = Util::StrToWStr(str);
	
	list<FontData*> glyphList;

	for (int i = 0; i < wStr.size(); i++)
	{
		glyphList.push_back(GetGlyphTexture(options.fontOptions, wStr.substr(i, 1)));
	}

	StringData strdata;

	strdata.width = 0;
	strdata.height = 0;

	strdata.width = -(*glyphList.begin())->gm.gmptGlyphOrigin.x;

	//最後の要素のイテレーター
	auto backitr = glyphList.end();
	backitr--;

	for (auto itr = glyphList.begin(); itr != glyphList.end(); itr++)
	{
		if (itr != backitr)
		{
			strdata.width += (*itr)->gm.gmCellIncX;
		}
		else
		{
			strdata.width += glyphList.back()->gm.gmptGlyphOrigin.x + glyphList.back()->gm.gmBlackBoxX;
		}

		strdata.height = max(strdata.height, (*itr)->tm.tmHeight);
	}

	UINT64 imageDataCount = strdata.width * strdata.height;
	R8G8B8A8* finalTexImageData = new R8G8B8A8[imageDataCount];

	for (UINT64 i = 0; i < imageDataCount; i++)
	{
		finalTexImageData[i] = { 0,0,0,0 };
	}

	//ダミーテクスチャ
	strdata.key = wTextureManager::CreateDummyTextureWithUniqueKey(strdata.width, strdata.height, str, false);

	int drawOriginX = 0;
	int drawOriginY = 0;
	int cellX = 0;

	//最後の文字のcellXに最後の文字のgmCellIncXを足した位置にさらにgmptGlyphOriginを足した位置が描画始点X
	//tmAscentからgmptGlyphOrigin.Yを引いた数値が文字の描画始点Y

	for (auto itr = glyphList.begin(); itr != glyphList.end(); itr++)
	{
		FontData* glyph = *itr;
		//最初の一文字は文字データの左端を原点とみなして描画
		if (itr == glyphList.begin())
		{
			//始点の計算
			drawOriginX = 0;
			drawOriginY = glyph->tm.tmAscent - glyph->gm.gmptGlyphOrigin.y;
		}
		//残りの文字は原点を基準に描画
		else
		{
			//始点の計算
			drawOriginX = cellX + glyph->gm.gmptGlyphOrigin.x;
			drawOriginY = glyph->tm.tmAscent - glyph->gm.gmptGlyphOrigin.y;
		}

		//ビットマップを埋める
		for (int i = 0; i < glyph->gm.gmBlackBoxY; i++)
		{
			for (int j = 0; j < glyph->gm.gmBlackBoxX; j++)
			{
				//indexの計算
				int x = drawOriginX + j;
				int y = drawOriginY + i;
				UINT64 index = y * strdata.width + x;

				//色を計算
				float alpha = (float)glyph->bmp[i * glyph->gm.gmBlackBoxX + j] / (float)glyph->grad * 255;
				//Float4 pixelColor = { alpha, alpha, alpha, alpha };

				R8G8B8A8 finalColor = {
					(BYTE)(alpha + finalTexImageData[index].r * (255 - alpha)),
					(BYTE)(alpha + finalTexImageData[index].g * (255 - alpha)),
					(BYTE)(alpha + finalTexImageData[index].b * (255 - alpha)),
					(BYTE)(alpha + finalTexImageData[index].a * (255 - alpha))
				};

				//書き込み
				finalTexImageData[index] = finalColor;
			}
		}

		//次のcellXを計算
		//最初の一文字
		if (itr == glyphList.begin())
		{
			cellX += -glyph->gm.gmptGlyphOrigin.x + glyph->gm.gmCellIncX;
		}
		//最初と最後以外
		else if (itr != backitr)
		{
			cellX += glyph->gm.gmCellIncX;
		}
	}

	wTextureManager::GetTextureBuff(strdata.key)->WriteToSubresource(
		0,
		nullptr,
		finalTexImageData,
		sizeof(R8G8B8A8) * strdata.width,
		sizeof(R8G8B8A8) * imageDataCount
	);

	delete[] finalTexImageData;

	return strdata;
}

void FontManager::InitData()
{
	GetInstance()->fontMap.clear();
	GetInstance()->fontNameMap.clear();
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

void TextDrawer::DrawString(string str, int x, int y, Align alignment, StringOptions options)
{
	StringData strData = FontManager::CreateStringTexture(str, options);

	if (strData.key == "") return;
	GetInstance()->stringSpriteQueue.emplace_back(strData.key);

	Sprite* spr = &GetInstance()->stringSpriteQueue.back();

	spr->position = { (float)x, (float)y, 0 };
	float multiplier = (float)options.size / strData.height;
	spr->scale = { multiplier, multiplier, 1};

	strData.width *= multiplier;
	strData.height *= multiplier;

	switch (alignment)
	{
	case Align::TopLeft:
		spr->position += {(float)strData.width/2, (float)strData.height/2, 0};
		break;
	case Align::TopRight:
		spr->position += {(float)-strData.width / 2, (float)strData.height / 2, 0};
		break;
	case Align::BottomLeft:
		spr->position += {(float)strData.width / 2, (float)-strData.height / 2, 0};
		break;
	case Align::BottomRight:
		spr->position += {(float)-strData.width / 2, (float)-strData.height / 2, 0};
		break;
	case Align::TopCenter:
		spr->position += {0, (float)strData.height / 2, 0};
		break;
	case Align::BottomCenter:
		spr->position += {0, (float)-strData.height / 2, 0};
		break;
	case Align::CenterLeft:
		spr->position += {(float)strData.width / 2, 0, 0};
		break;
	case Align::CenterRight:
		spr->position += {(float)-strData.width / 2, 0, 0};
		break;
	case Align::Center:
		break;
	default:
		break;
	}

	spr->UpdateMatrix();

	spr->Draw();

	GetInstance()->releaseQueue.push_back(strData.key);
}

void TextDrawer::DrawString(string str, int x, int y, Align alignment)
{
	DrawString(str, x, y, alignment, GetInstance()->defaultOption);
}

void TextDrawer::ReleaseDrawStringData()
{
	for (auto texKey : GetInstance()->releaseQueue)
	{
		wTextureManager::Release(texKey);
	}

	GetInstance()->releaseQueue.clear();
	GetInstance()->stringSpriteQueue.clear();
}

TextDrawer* TextDrawer::GetInstance()
{
	static TextDrawer obj;
	return &obj;
}
