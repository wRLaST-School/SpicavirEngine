#pragma once
#include "Essentials.h"
//TODO:‚à‚¤‚¿‚å‚¢‰½‚Æ‚©‚·‚é
class wTexture
{
public:
	//ŽÀŽ¿‚½‚¾‚Ìinit
	void LoadTexture(string path);

	ComPtr<ID3D12Resource> texBuff = nullptr;
	D3D12_RESOURCE_DESC texresdesc{};
};

void InitTextureBuff();
wTexture* GetTexture();
