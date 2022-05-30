#pragma once
#include "Essentials.h"
class wTextureManager
{
public:
	void Init();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
};

void CreateAndInitTextureManager();
wTextureManager* GetTextureManager();

