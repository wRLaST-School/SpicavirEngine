#pragma once
#include "Essentials.h"
class SpDepth
{
public:
	void Init();
	void Resize();

	ID3D12Resource* depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
};

void InitSpDepth();
SpDepth* GetSpDepth();

