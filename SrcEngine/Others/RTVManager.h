#pragma once
#include "SpTextureManager.h"
#include "RenderTarget.h"
class RTVManager
{
public:
	static void SetRenderTargetToBackBuffer(UINT bbIndex);
	static void SetRenderTargetToTexture(const TextureKey& key, bool clear = true);
	static void SetRenderTargets(const std::vector<TextureKey>& keys);
	static void SetRenderTargetToCurrentBB();

	static void CreateRenderTargetTexture(int32_t width, int32_t height, const TextureKey& key);
	static void CreateRenderTargetTexture(float width, float height, const TextureKey& key, bool useScreenRatio = false);
	static void CreateHeaps();

	static int32_t GetCurrentRenderTarget();

	static void ClearCurrentRenderTarget(const Float4& color);

	static RTVManager& GetInstance();

	static D3D12_CPU_DESCRIPTOR_HANDLE GetHeapCPUHandle(int32_t index);

	static void CloseCurrentResBar();

	const int32_t numRT = 256;

	bool isAllResBarClosed = true;

private:
	int32_t currentRTIndex_[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps_ = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc_{};
};

