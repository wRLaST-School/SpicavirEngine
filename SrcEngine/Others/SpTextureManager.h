#pragma once
#include "Essentials.h"
#include <exc_unordered_map.h>

typedef std::string TextureKey;
typedef size_t SRVHeapIndex;
//index��unsigned char�ɂȂ��Ă邩�炱����ύX����Ȃ炻�������ύX
const size_t wMaxSRVCount = 1024;

class SpTextureManager
{
private:
	struct TexData {
		TexMetadata meta;
		Float2 ratio = { 0.f, 0.f }; //x���[���ȊO�Ȃ��ʔ䗦
	};
public:
	static void Create();
	static void Init();
	static TextureKey LoadTexture(string filePath, TextureKey key);
	static TextureKey LoadTextureWithUniqueKey(string filePath, TextureKey key);
	static TextureKey CreateDummyTexture(float width, float height, TextureKey key, bool initAsRenderTarget = true, bool useRatio = false);
	static TextureKey CreateDummyTextureWithUniqueKey(int32_t width, int32_t height, TextureKey key, bool initAsRenderTarget = true);

	static void LoadDiv(string filePath, int32_t widthPer, int32_t heightPer, int32_t qx, int32_t qy, const vector<TextureKey>& keys);
	static TextureKey LoadSingleDiv(string filePath, int32_t originX, int32_t originY, int32_t width, int32_t height, TextureKey key);

	static TextureKey CreatePlainSRV(TextureKey key);
	
	static void ResizeScreenTextures();

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(TextureKey key);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(TextureKey key);
	static TexMetadata GetTextureMetadata(TextureKey key);
	static TexData GetTextureData(TextureKey key);
	static ID3D12Resource* GetTextureBuff(TextureKey key);
	static int32_t GetIndex(TextureKey key);

	//�V�[���𒴂��Ďg�p���郊�\�[�X�Ƃ��Đݒ�
	static void AddMasterTextureKey(TextureKey key);

	static void Release(TextureKey key);
	//�O�̃V�[���Ŏg���Ă��č��̃V�[���Ŏg���Ă��Ȃ��e�N�X�`���������[�X
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
	static int32_t currentSceneResIndex;

	static list<TextureKey> masterTextures;
private:
	SpTextureManager() {};
	SpTextureManager(const SpTextureManager&) = delete;
	SpTextureManager& operator= (const SpTextureManager&) = delete;
	~SpTextureManager() {};
};