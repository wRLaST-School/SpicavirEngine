#pragma once
#include "Essentials.h"
class RenderTarget
{
	ID3D12Resource* targetBuff;
	D3D12_RENDER_TARGET_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle;
};

