#pragma once
#include "Essentials.h"
class wRootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature> rootsignature;
};

wRootSignature* GetRootSignature();
void SetRootSignature(wRootSignature rs);
void CreateAndInitRootSignature();

