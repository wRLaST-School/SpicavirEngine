#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
class SpShader
{
public:
	void InitVS(string vsPath);
	void InitPS(string psPath);
	void InitGS(string gsPath);

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob = nullptr; // ピクセルシェーダオブジェクト
};

void RegisterShader(string id);
void InitVS(string id, string vsPath);
void InitPS(string id, string psPath);
void InitGS(string id, string gsPath);
SpShader* GetShader(string id);

