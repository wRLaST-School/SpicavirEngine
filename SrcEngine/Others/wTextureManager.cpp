#include "wTextureManager.h"
#include "wDirectX.h"

void wTextureManager::Create()
{
	GetInstance().nextRegisteredTextureIndex = 0;
	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = wMaxSRVCount;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&GetInstance().srvHeap));
}

void wTextureManager::Init()
{
	wTextureManager& ins = GetInstance();
	ins.nextRegisteredTextureIndex = 0;

	for (int i = 0; i < wMaxSRVCount; i++)
	{
		ins.isOccupied[i] = false;
	}
	
	ins.textureMap.clear();

	for (ComPtr<ID3D12Resource>& texbuffs : ins.texBuffs)
	{
		texbuffs = nullptr;
	}

	wTextureManager::LoadTexture("Resources/notexture.png", "notexture");
}

TextureKey wTextureManager::LoadTexture(string filePath, TextureKey key)
{
	wTextureManager& ins = GetInstance();
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
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]->WriteToSubresource(
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
	heapHandle = wTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().nextRegisteredTextureIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex].Get(), &srvDesc, heapHandle);
	
	wTextureManager::GetInstance().textureMap[key] = wTextureManager::GetInstance().nextRegisteredTextureIndex;
	wTextureManager::GetInstance().texDataMap[key] = metadata;
	GetInstance().isOccupied[GetInstance().nextRegisteredTextureIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextRegisteredTextureIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
	return key;
}

TextureKey wTextureManager::LoadTextureWithUniqueKey(string filePath, TextureKey key)
{
	wTextureManager& ins = GetInstance();
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
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]->WriteToSubresource(
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
	heapHandle = wTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().nextRegisteredTextureIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex].Get(), &srvDesc, heapHandle);

	int tryNum = 0;
	bool succeeded = false;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		string tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		succeeded = wTextureManager::GetInstance().textureMap.try_emplace(tryKey, wTextureManager::GetInstance().nextRegisteredTextureIndex).second;
	}
	wTextureManager::GetInstance().texDataMap[tryNum == 0 ? key : key + std::to_string(tryNum)] = metadata;

	wTextureManager::GetInstance().textureMap[key] = wTextureManager::GetInstance().nextRegisteredTextureIndex;
	wTextureManager::GetInstance().texDataMap[key] = metadata;
	GetInstance().isOccupied[GetInstance().nextRegisteredTextureIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextRegisteredTextureIndex = i;
			return tryNum == 0 ? key : key + std::to_string(tryNum);
		}
	}

	throw std::out_of_range("out of texture resource");

	return tryNum == 0 ? key : key + std::to_string(tryNum);
}

TextureKey wTextureManager::CreateDummyTexture(int width, int height, TextureKey key, bool initAsRenderTarget)
{
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc = 
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1, 1,0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]));

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = wTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().nextRegisteredTextureIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex].Get(), &srvDesc, heapHandle);
	wTextureManager::GetInstance().textureMap[key] = wTextureManager::GetInstance().nextRegisteredTextureIndex;
	TexMetadata& pTexMeta = wTextureManager::GetInstance().texDataMap[key];
	pTexMeta = TexMetadata{};
	pTexMeta.width = width;
	pTexMeta.height = height;

	GetInstance().isOccupied[GetInstance().nextRegisteredTextureIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextRegisteredTextureIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
	return key;
}

TextureKey wTextureManager::CreateDummyTextureWithUniqueKey(int width, int height, TextureKey key, bool initAsRenderTarget)
{//テクスチャバッファ
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
		initAsRenderTarget ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]));

	//シェーダーリソースビューの生成
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = wTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().nextRegisteredTextureIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex].Get(), &srvDesc, heapHandle);
	
	int tryNum = 0;
	bool succeeded = false;
	string tryKey;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		succeeded = wTextureManager::GetInstance().textureMap.try_emplace(tryKey, wTextureManager::GetInstance().nextRegisteredTextureIndex).second;
	}
	TexMetadata& pTexMeta = wTextureManager::GetInstance().texDataMap[tryKey];
	pTexMeta = TexMetadata{};
	pTexMeta.width = width;
	pTexMeta.height = height;

	GetInstance().isOccupied[GetInstance().nextRegisteredTextureIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied[i])
		{
			GetInstance().nextRegisteredTextureIndex = i;
			return tryKey;
		}
	}

	throw std::out_of_range("out of texture resource");
	return tryKey;
}

D3D12_CPU_DESCRIPTOR_HANDLE wTextureManager::GetCPUDescHandle(TextureKey key)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = wTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().textureMap[key];
	return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE wTextureManager::GetGPUDescHandle(TextureKey key)
{
	D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = wTextureManager::GetInstance().srvHeap->GetGPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * wTextureManager::GetInstance().textureMap[key];
	return heapHandle;
}

TexMetadata wTextureManager::GetTextureMetadata(TextureKey key)
{
	return wTextureManager::GetInstance().texDataMap[key];
}

ID3D12Resource* wTextureManager::GetTextureBuff(TextureKey key)
{
	return wTextureManager::GetInstance().texBuffs[wTextureManager::GetInstance().textureMap[key]].Get();
}

int wTextureManager::GetIndex(TextureKey key)
{
	return GetInstance().textureMap[key];
}

void wTextureManager::Release(TextureKey key)
{

	wTextureManager& ins = GetInstance();
	ins.texBuffs[ins.textureMap[key]] = ComPtr<ID3D12Resource>();
	ins.isOccupied[GetIndex(key)] = false;
	ins.textureMap.erase(key);
	ins.texDataMap.erase(key);
	return;
}

wTextureManager &wTextureManager::GetInstance()
{
	static wTextureManager obj;
	return obj;
}