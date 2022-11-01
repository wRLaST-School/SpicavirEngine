#pragma once
#include "Essentials.h"
class SpDepth
{
public:
	void Init();
	void Resize();

	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
};

void InitWDepth();
SpDepth* GetWDepth();

