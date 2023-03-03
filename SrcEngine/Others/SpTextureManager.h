#pragma once
#include "Essentials.h"
#include <exc_unordered_map.h>

typedef std::string TextureKey;
typedef size_t SRVHeapIndex;
//indexがunsigned charになってるからここを変更するならそっちも変更
const size_t wMaxSRVCount = 1024;

class SpTextureManager
{
private:
	struct TexData {
		TexMetadata meta;
		Float2 ratio = { 0.f, 0.f }; //xがゼロ以外なら画面比率
	};
public:
	static void Create();
	static void Init();
	static TextureKey LoadTexture(string filePath, TextureKey key);
	static TextureKey LoadTextureWithUniqueKey(string filePath, TextureKey key);
	static TextureKey CreateDummyTexture(float width, float height, TextureKey key, bool initAsRenderTarget = true, bool useRatio = false);
	static TextureKey CreateDummyTextureWithUniqueKey(int width, int height, TextureKey key, bool initAsRenderTarget = true);

	static TextureKey CreatePlainSRV(TextureKey key);

	static void ResizeScreenTextures();

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(TextureKey key);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(TextureKey key);
	static TexMetadata GetTextureMetadata(TextureKey key);
	static TexData GetTextureData(TextureKey key);
	static ID3D12Resource* GetTextureBuff(TextureKey key);
	static int GetIndex(TextureKey key);

	//シーンを超えて使用するリソースとして設定
	static void AddMasterTextureKey(TextureKey key);

	static void Release(TextureKey key);
	//前のシーンで使われていて今のシーンで使われていないテクスチャをリリース
	static void ReleasePerSceneTexture();
	static void PreLoadNewScene();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	static SpTextureManager &GetInstance();

	ComPtr<ID3D12Resource>texBuffs[wMaxSRVCount] = {};

private:
	size_t nextTexIndex = 0;
	exc_unordered_map<TextureKey, SRVHeapIndex> textureMap = {};
	exc_unordered_map<TextureKey, TexData> texDataMap = {};
	bool isOccupied[wMaxSRVCount] = {};

	static list<TextureKey> perSceneTextures[2];
	static int currentSceneResIndex;

	static list<TextureKey> masterTextures;
private:
	SpTextureManager() {};
	SpTextureManager(const SpTextureManager&) = delete;
	SpTextureManager& operator= (const SpTextureManager&) = delete;
	~SpTextureManager() {};
};