#pragma once
#include "Essentials.h"
//TODO:もうちょい何とかする
class wTexture
{
public:
	//実質ただのinit
	void LoadTexture(string path);

	ComPtr<ID3D12Resource> texBuff = nullptr;
	D3D12_RESOURCE_DESC texresdesc{};
};

void InitTextureBuff();
wTexture* GetTexture();
