#include "wTexture.h"
#include "wDirectX.h"

wTexture wtex;

void wTexture::LoadTexture(string path)
{
	
}

void InitTextureBuff()
{
	wtex.LoadTexture("Resources/think.png");
}

wTexture* GetTexture()
{
	return &wtex;
}
