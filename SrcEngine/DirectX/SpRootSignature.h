#pragma once
#include "Essentials.h"

class SpRootSignature
{
public:
	void Create();
	void UseDefaultSettings();

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
	std::vector<CD3DX12_ROOT_PARAMETER> params;
	D3D12_STATIC_SAMPLER_DESC samplerDesc;

public:
	static SpRootSignature* Get(const std::string& id);
	static SpRootSignature* Register(const std::string& id);
	static std::map<std::string, SpRootSignature> sRsMap;
};