#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
class SpShader
{
public:
	void InitVS(const std::string& vsPath);
	void InitPS(const std::string& psPath);
	void InitGS(const std::string& gsPath);

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
};

void RegisterShader(const std::string& id);
void InitVS(const std::string& id, const std::string& vsPath);
void InitPS(const std::string& id, const std::string& psPath);
void InitGS(const std::string& id, const std::string& gsPath);
SpShader* GetShader(const std::string& id);

