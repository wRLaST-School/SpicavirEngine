#pragma once
#include "Essentials.h"

typedef std::string TextureKey;
typedef size_t SRVHeapIndex;
//index��unsigned char�ɂȂ��Ă邩�炱����ύX����Ȃ炻�������ύX
const size_t wMaxSRVCount = 1024;

class wTextureManager final
{
public:
	static void Create();
	static void Init();
	static TextureKey LoadTexture(string filePath, TextureKey key);
	static TextureKey LoadTextureWithUniqueKey(string filePath, TextureKey key);
	static TextureKey CreateDummyTexture(int width, int height, TextureKey key, bool initAsRenderTarget = true);
	static TextureKey CreateDummyTextureWithUniqueKey(int width, int height, TextureKey key, bool initAsRenderTarget = true);

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(TextureKey key);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(TextureKey key);
	static TexMetadata GetTextureMetadata(TextureKey key);
	static ID3D12Resource* GetTextureBuff(TextureKey key);
	static int GetIndex(TextureKey key);

	static void Release(TextureKey key);

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	static wTextureManager &GetInstance();

	ComPtr<ID3D12Resource>texBuffs[wMaxSRVCount] = {};

private:
	size_t nextRegisteredTextureIndex = 0;
	map<TextureKey, SRVHeapIndex> textureMap = {};
	map<TextureKey, TexMetadata> texDataMap = {};
	bool isOccupied[wMaxSRVCount] = {};

private:
	wTextureManager() {};
	wTextureManager(const wTextureManager&) = delete;
	wTextureManager& operator= (const wTextureManager&) = delete;
	~wTextureManager() {};
};