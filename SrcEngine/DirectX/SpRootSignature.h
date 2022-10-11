#pragma once
#include "Essentials.h"
class SpRootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature> rootsignature;
};

SpRootSignature* GetRootSignature();
void SetRootSignature(SpRootSignature rs);
void CreateAndInitRootSignature();

