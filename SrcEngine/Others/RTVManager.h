#pragma once
#include "SpTextureManager.h"
#include "RenderTarget.h"
class RTVManager
{
public:
	static void SetRenderTargetToBackBuffer(UINT bbIndex);
	static void SetRenderTargetToTexture(TextureKey key, bool clear = true);
	static void SetRenderTargets(vector<TextureKey> keys);
	static void SetRenderTargetToCurrentBB();

	static void CreateRenderTargetTexture(int width, int height, TextureKey key);
	static void CreateRenderTargetTexture(float width, float height, TextureKey key, bool useScreenRatio = false);
	static void CreateHeaps();

	static int GetCurrentRenderTarget();

	static void ClearCurrentRenderTarget(Float4 color);

	static RTVManager& GetInstance();

	static D3D12_CPU_DESCRIPTOR_HANDLE GetHeapCPUHandle(int index);

	static void CloseCurrentResBar();

	const int numRT = 256;

	bool isAllResBarClosed = true;

private:
	int currentRTIndex[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
};

