#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
class SpShader
{
public:
	void InitVS(const string& vsPath);
	void InitPS(const string& psPath);
	void InitGS(const string& gsPath);

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
};

void RegisterShader(const string& id);
void InitVS(const string& id, const string& vsPath);
void InitPS(const string& id, const string& psPath);
void InitGS(const string& id, const string& gsPath);
SpShader* GetShader(const string& id);

