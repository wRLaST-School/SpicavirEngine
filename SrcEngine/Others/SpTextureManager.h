#pragma once
/*****************************************************************//**
 * @file   SpTextureManager.h
 * @brief  テクスチャの読み込みや取得に関するヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include "Essentials.h"
#include <exc_unordered_map.h>

typedef std::string TextureKey;
typedef size_t SRVHeapIndex;
//indexがunsigned charになってるからここを変更するならそっちも変更
const size_t wMaxSRVCount = 1024;

class ResourceWindow;
class SceneRW;

/**
 * @brief テクスチャの読み込みや取得に関するクラス
 */
class SpTextureManager
{
private:
	struct TexData {
		DirectX::TexMetadata meta;
		Float2 ratio = { 0.f, 0.f }; //xがゼロ以外なら画面比率
		std::string filePath;
	};
public:
	//リソースウィンドウに一覧を表示するため
	friend ResourceWindow;
	//シーン情報書き込みで使用するため
	friend SceneRW;

	static void Create();
	static void Init();
	/**
	 * @brief テクスチャの読み込み
	 * 
	 * @param filePath ファイルパス
	 * @param key テクスチャの登録キー
	 * @return 登録したキー
	 */
	static TextureKey LoadTexture(const std::string& filePath, const TextureKey& key);
	
	/**
	* @brief テクスチャの読み込み
	*
	* @param filePath ファイルパス
	* @param key テクスチャの登録キー
	* @return 登録したキー
	*/
	static TextureKey LoadTextureWithUniqueKey(const std::string& filePath, const TextureKey& key);
	
	/**
	 * @brief ダミーテクスチャの生成
	 * 
	 * @param width 横幅
	 * @param height 縦幅
	 * @param key テクスチャの登録キー
	 * @param initAsRenderTarget レンダーターゲットとして初期化するか
	 * @param useRatio widthとheightを画面サイズに対する比率で扱うか
	 * @return 登録したキー
	 */
	static TextureKey CreateDummyTexture(float width, float height, const TextureKey& key, bool initAsRenderTarget = true, bool useRatio = false);
	
	/**
	 * @brief ダミーテクスチャの生成
	 *
	 * @param width 横幅
	 * @param height 縦幅
	 * @param key テクスチャの登録キー
	 * @param initAsRenderTarget レンダーターゲットとして初期化するか
	 * @param useRatio widthとheightを画面サイズに対する比率で扱うか
	 * @return 登録したキー
	 */
	static TextureKey CreateDummyTextureWithUniqueKey(int32_t width, int32_t height, const TextureKey& key, bool initAsRenderTarget = true);

	/**
	 * @brief 画像の分割読み込み
	 * 
	 * @param filePath ファイルパス
	 * @param widthPer 一つあたりの横幅
	 * @param heightPer 一つあたりの縦幅
	 * @param qx x方向の数
	 * @param qy y方向の数
	 * @param keys 登録するキーの配列
	 */
	static void LoadDiv(const std::string& filePath, int32_t widthPer, int32_t heightPer, int32_t qx, int32_t qy, const std::vector<TextureKey>& keys);
	
	/**
	 * @brief 画像の分割読み込み(一つのみ)
	 * 
	 * @param filePath ファイルパス
	 * @param originX 原点x
	 * @param originY 原点y
	 * @param width 原点からの横幅
	 * @param height 原点からの縦幅
	 * @param key テクスチャの登録キー
	 * @return 登録したキー
	 */
	static TextureKey LoadSingleDiv(std::string filePath, int32_t originX, int32_t originY, int32_t width, int32_t height, const TextureKey& key);

	static TextureKey CreatePlainSRV(const TextureKey& key);

	static void ResizeScreenTextures();

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(const TextureKey& key);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(const TextureKey& key);
	static DirectX::TexMetadata GetTextureMetadata(const TextureKey& key);
	static TexData GetTextureData(const TextureKey& key);
	static ID3D12Resource* GetTextureBuff(const TextureKey& key);
	static int32_t GetIndex(const TextureKey& key);

	//シーンを超えて使用するリソースとして設定
	static void AddMasterTextureKey(const TextureKey& key);

	static bool IsMasterTexture(const TextureKey& key);

	static void Release(const TextureKey& key);
	//前のシーンで使われていて今のシーンで使われていないテクスチャをリリース
	static void ReleasePerSceneTexture();
	static void PreLoadNewScene();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	static SpTextureManager& GetInstance();

	Microsoft::WRL::ComPtr<ID3D12Resource>texBuffs[wMaxSRVCount] = {};

private:
	size_t nextTexIndex_ = 0;
	exc_unordered_map<TextureKey, SRVHeapIndex> textureMap_ = {};
	exc_unordered_map<TextureKey, TexData> texDataMap_ = {};
	bool isOccupied_[wMaxSRVCount] = {};

	static std::list<TextureKey> sPerSceneTextures[2];
	static int32_t sCurrentSceneResIndex;

	static std::list<TextureKey> sMasterTextures;

	static std::wstring GetExtension(const std::wstring& path);
private:
	SpTextureManager() {};
	SpTextureManager(const SpTextureManager&) = delete;
	SpTextureManager& operator= (const SpTextureManager&) = delete;
	~SpTextureManager() {};
};