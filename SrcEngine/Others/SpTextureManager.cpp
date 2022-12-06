#include "SpTextureManager.h"
#include "SpDirectX.h"

#define SPTEX_NOTEXTURE_FOUND_ -1024

void SpTextureManager::Create()
{
	GetInstance().nextTexIndex = 0;
	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = wMaxSRVCount;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&GetInstance().srvHeap));
}

void SpTextureManager::Init()
{
	SpTextureManager& ins = GetInstance();
	ins.nextTexIndex = 0;

	for (int i = 0; i < wMaxSRVCount; i++)
	{
		ins.isOccupied[i] = false;
	}

	ins.textureMap.Access([&](auto& map) {
		map.clear();
		});

	for (ComPtr<ID3D12Resource>& texbuffs : ins.texBuffs)
	{
		texbuffs = nullptr;
	}

	SpTextureManager::LoadTexture("Resources/notexture.png", "notexture");
	AddMasterTextureKey("notexture");
}

TextureKey SpTextureManager::LoadTexture(string filePath, TextureKey key)
{
	perSceneTextures[currentSceneResIndex].push_back(key);
	bool alreadyRegistered = false;
	GetInstance().textureMap.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});
	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exists. skipping.") + string("\n")).c_str());
		return key;
	}
	OutputDebugStringA((string("Loading : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex) + string(")\n")).c_str());
	SpTextureManager& ins = GetInstance();
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//metadata.format = MakeSRGB(metadata.format);

	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = metadata.arraySize;
	texresdesc.MipLevels = metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	HRESULT hr = GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = GetInstance().texBuffs[GetInstance().nextTexIndex]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex].Get(), &srvDesc, heapHandle);

	SpTextureManager::GetInstance().textureMap.Access(
		[&](auto& map) {
			map[key] = SpTextureManager::GetInstance().nextTexIndex;
		}
	);

	SpTextureManager::GetInstance().texDataMap.Access(
		[&](auto& map) {
			map[key] = metadata;
		}
	);

	GetInstance().isOccupied[GetInstance().nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextTexIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
	return key;
}

TextureKey SpTextureManager::LoadTextureWithUniqueKey(string filePath, TextureKey key)
{
	OutputDebugStringA((string("Loading Unique : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex) + string(")\n")).c_str());
	SpTextureManager& ins = GetInstance();
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//metadata.format = MakeSRGB(metadata.format);

	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;


	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = metadata.arraySize;
	texresdesc.MipLevels = metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = GetInstance().texBuffs[GetInstance().nextTexIndex]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex].Get(), &srvDesc, heapHandle);

	int tryNum = 0;
	bool succeeded = false;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		string tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		SpTextureManager::GetInstance().textureMap.Access(
			[&](auto& map) {
				succeeded = map.try_emplace(tryKey, SpTextureManager::GetInstance().nextTexIndex).second;
			}
		);
	}

	ins.textureMap.Access(
		[&](auto& map) {
			map[tryNum == 0 ? key : key + std::to_string(tryNum)] = ins.nextTexIndex;
		}
	);

	ins.texDataMap.Access(
		[&](auto& map) {
			map[tryNum == 0 ? key : key + std::to_string(tryNum)] = metadata;
		}
	);

	GetInstance().isOccupied[GetInstance().nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextTexIndex = i;
			perSceneTextures[currentSceneResIndex].push_back(tryNum == 0 ? key : key + std::to_string(tryNum));
			return tryNum == 0 ? key : key + std::to_string(tryNum);
		}
	}

	throw std::out_of_range("out of texture resource");

	return tryNum == 0 ? key : key + std::to_string(tryNum);
}

TextureKey SpTextureManager::CreateDummyTexture(int width, int height, TextureKey key, bool initAsRenderTarget)
{
	perSceneTextures[currentSceneResIndex].push_back(key);
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i] && i <= 253)
		{
			GetInstance().nextTexIndex = i;
			perSceneTextures[currentSceneResIndex].push_back(key);
			break;
		}
	}

	bool alreadyRegistered = false;
	GetInstance().textureMap.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});

	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exist. skipping.") + string("\n")).c_str());
		return key;
	}

	OutputDebugStringA((string("Creating Dummy Tex U: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex) + string("\n")).c_str());
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex].Get(), &srvDesc, heapHandle);

	SpTextureManager::GetInstance().textureMap.Access(
		[&](auto& map) {
			map[key] = SpTextureManager::GetInstance().nextTexIndex;
		}
	);

	SpTextureManager::GetInstance().texDataMap.Access(
		[&](auto& map) {
			TexMetadata& pTexMeta = map[key];
			pTexMeta = TexMetadata{};
			pTexMeta.width = width;
			pTexMeta.height = height;
		}
	);

	GetInstance().isOccupied[GetInstance().nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextTexIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
	return key;
}

TextureKey SpTextureManager::CreateDummyTextureWithUniqueKey(int width, int height, TextureKey key, bool initAsRenderTarget)
{//テクスチャバッファ
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i] && i <= 253)
		{
			GetInstance().nextTexIndex = i;
			perSceneTextures[currentSceneResIndex].push_back(key);
			break;
		}
	}
	OutputDebugStringA((string("Creating Dummy Tex U: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex) + string("\n")).c_str());
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex].Get(), &srvDesc, heapHandle);

	int tryNum = 0;
	bool succeeded = false;
	string tryKey;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		SpTextureManager::GetInstance().textureMap.Access(
			[&](auto& map) {
				succeeded = map.try_emplace(tryKey, SpTextureManager::GetInstance().nextTexIndex).second;
			}
		);
	}
	SpTextureManager::GetInstance().texDataMap.Access(
		[&](auto& map) {
			TexMetadata& pTexMeta = map[key];
			pTexMeta = TexMetadata{};
			pTexMeta.width = width;
			pTexMeta.height = height;
		}
	);

	GetInstance().isOccupied[GetInstance().nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextTexIndex = i;
			perSceneTextures[currentSceneResIndex].push_back(tryKey);
			return tryKey;
		}
	}

	throw std::out_of_range("out of texture resource");
	return tryKey;
}

TextureKey SpTextureManager::CreatePlainSRV(TextureKey key)
{
	perSceneTextures[currentSceneResIndex].push_back(key);
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i] && i <= 253)
		{
			GetInstance().nextTexIndex = i;
			perSceneTextures[currentSceneResIndex].push_back(key);
			break;
		}
	}

	bool alreadyRegistered = false;
	GetInstance().textureMap.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});

	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exist. skipping.") + string("\n")).c_str());
		return key;
	}

	OutputDebugStringA((string("Creating Plain SRV: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex) + string("\n")).c_str());

	SpTextureManager::GetInstance().textureMap.Access(
		[&](auto& map) {
			map[key] = SpTextureManager::GetInstance().nextTexIndex;
		}
	);

	SpTextureManager::GetInstance().texDataMap.Access(
		[&](auto& map) {
			TexMetadata& pTexMeta = map[key];
			pTexMeta = TexMetadata{};
			pTexMeta.width = -1;
			pTexMeta.height = -1;
		}
	);

	GetInstance().isOccupied[GetInstance().nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextTexIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
	return key;
}

D3D12_CPU_DESCRIPTOR_HANDLE SpTextureManager::GetCPUDescHandle(TextureKey key)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	GetInstance().textureMap.Access(
		[&](auto& map) {
			heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * map[key];

		}
	);
	return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SpTextureManager::GetGPUDescHandle(TextureKey key)
{
	D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetGPUDescriptorHandleForHeapStart();
	GetInstance().textureMap.Access(
		[&](auto& map) {
			heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * map[key];

		}
	);
	return heapHandle;
}

TexMetadata SpTextureManager::GetTextureMetadata(TextureKey key)
{
	TexMetadata meta;
	GetInstance().texDataMap.Access(
		[&](auto& map) {
			meta = map[key];
		}
	);
	return meta;
}

ID3D12Resource* SpTextureManager::GetTextureBuff(TextureKey key)
{
	SRVHeapIndex index = 114514;
	GetInstance().textureMap.Access(
		[&](auto& map) {
			index = map[key];
		}
	);
	return SpTextureManager::GetInstance().texBuffs[index].Get();
}

int SpTextureManager::GetIndex(TextureKey key)
{
	int ret = 0;
	GetInstance().textureMap.Access(
		[&](auto& map) {
			auto itr = map.find(key);
			if (itr == map.end()) ret = SPTEX_NOTEXTURE_FOUND_;
			else ret = itr->second;
		}
	);

	return ret;
}

void SpTextureManager::AddMasterTextureKey(TextureKey key)
{
	masterTextures.push_back(key);
}

void SpTextureManager::Release(TextureKey key)
{
	SpTextureManager& ins = GetInstance();

	OutputDebugStringA((string("Unloading : ") + key + string(" (Heap Index : ") + to_string(GetIndex(key)) + string("\n")).c_str());
	if (GetIndex(key) == SPTEX_NOTEXTURE_FOUND_) {
		OutputDebugStringA("No Texture Found.\n");
		return;
	}

	ins.isOccupied[GetIndex(key)] = false;
	ins.textureMap.Access(
		[&](auto& map) {
			ins.texBuffs[map[key]] = ComPtr<ID3D12Resource>();
			map.erase(key);
		}
	);

	ins.texDataMap.Access(
		[&](auto& map) {
			map.erase(key);
		}
	);
	return;
}

void SpTextureManager::ReleasePerSceneTexture()
{
	int lastSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
	for (auto itr = perSceneTextures[lastSceneResIndex].begin(); itr != perSceneTextures[lastSceneResIndex].end(); itr++)
	{
		bool usingInCurrentScene = false;
		//次のシーンでも使われるかチェック
		for (auto key : perSceneTextures[currentSceneResIndex])
		{
			if (key == *itr)
			{
				usingInCurrentScene = true;
				break;
			}
		}

		//マスターテクスチャかチェック
		for (auto& tk : masterTextures)
		{
			if (*itr == tk)
			{
				usingInCurrentScene = true;
			}
		}

		if (!usingInCurrentScene) //今のシーンで使われていないならリリース
		{
			Release(*itr);
		}
	}
	perSceneTextures[lastSceneResIndex].clear();
}

void SpTextureManager::PreLoadNewScene()
{
	currentSceneResIndex = currentSceneResIndex == 0 ? 1 : 0;
}

SpTextureManager& SpTextureManager::GetInstance()
{
	static SpTextureManager obj;
	return obj;
}

list<TextureKey> SpTextureManager::perSceneTextures[2] = {};
int SpTextureManager::currentSceneResIndex = 0;

list<TextureKey> SpTextureManager::masterTextures;