#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
class SpShader
{
public:
	void InitVS(const string& vsPath);
	void InitPS(const string& psPath);
	void InitGS(const string& gsPath);

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob = nullptr; // ピクセルシェーダオブジェクト
};

void RegisterShader(const string& id);
void InitVS(const string& id, const string& vsPath);
void InitPS(const string& id, const string& psPath);
void InitGS(const string& id, const string& gsPath);
SpShader* GetShader(const string& id);

