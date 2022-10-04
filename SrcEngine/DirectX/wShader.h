#pragma once
#include "Essentials.h"
#include "wDirectX.h"
class wShader
{
public:
	void Init(string vsPath, string psPath);

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
};

void RegisterAndInitShader(string id, string vsPath, string psPath);
wShader* GetShader(string id);

