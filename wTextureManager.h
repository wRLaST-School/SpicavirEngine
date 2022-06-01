#pragma once
#include "Essentials.h"

typedef std::string TextureKey;
typedef unsigned char SRVHeapIndex;

class wTextureManager final
{
public:
	static void Create();
	static void Init();
	static TextureKey LoadTexture(string filePath, TextureKey key);

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(TextureKey key);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(TextureKey key);

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	static wTextureManager &GetInstance();

	vector<ComPtr<ID3D12Resource>> texBuffs = {};

private:
	int nextRegisteredTextureIndex = 0;
	map<TextureKey, SRVHeapIndex> textureMap = {};

private:
	wTextureManager() {};
	wTextureManager(const wTextureManager&) = delete;
	wTextureManager& operator= (const wTextureManager&) = delete;
	~wTextureManager() {};
};