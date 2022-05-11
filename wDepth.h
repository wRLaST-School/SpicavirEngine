#pragma once
#include "Essentials.h"
class wDepth
{
public:
	void Init();

	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
};

void InitWDepth();
wDepth* GetWDepth();

