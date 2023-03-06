#include "SpTextureManager.h"
#include "SpDirectX.h"
#include <RTVManager.h>

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
	ScratchImage srcImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, srcImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(srcImg.GetImages(), srcImg.GetImageCount(), srcImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		srcImg = move(mipChain);
		metadata = srcImg.GetMetadata();
	}

	//metadata.format = MakeSRGB(metadata.format);
	ScratchImage scratchImg;
	result = Convert(srcImg.GetImages(), srcImg.GetImageCount(),
		srcImg.GetMetadata(),
		DXGI_FORMAT_R16G16B16A16_FLOAT, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT,
		scratchImg);

	if (SUCCEEDED(result)) {
		metadata = scratchImg.GetMetadata();
	}

	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresdesc.MipLevels = (UINT16)metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	HRESULT hr = GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ins.texBuffs[ins.nextTexIndex]));

	if (hr != S_OK)
	{
		return string("notexture");
	};

	ins.texBuffs[ins.nextTexIndex]->SetName(L"TEXTURE_BUFFER");

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		ins.texBuffs[ins.nextTexIndex]->WriteToSubresource(
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
	heapHandle = ins.srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(ins.texBuffs[ins.nextTexIndex].Get(), &srvDesc, heapHandle);

	ins.textureMap.Access(
		[&](auto& map) {
			map[key] = ins.nextTexIndex;
		}
	);

	ins.texDataMap.Access(
		[&](auto& map) {
			map[key].meta = metadata;
		}
	);

	ins.isOccupied[ins.nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!ins.isOccupied[i])
		{
			ins.nextTexIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
}

TextureKey SpTextureManager::LoadTextureWithUniqueKey(string filePath, TextureKey key)
{
	OutputDebugStringA((string("Loading Unique : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex) + string(")\n")).c_str());
	SpTextureManager& ins = GetInstance();
	TexMetadata metadata{};
	ScratchImage srcImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, srcImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(srcImg.GetImages(), srcImg.GetImageCount(), srcImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		srcImg = move(mipChain);
		metadata = srcImg.GetMetadata();
	}

	//metadata.format = MakeSRGB(metadata.format);
	ScratchImage scratchImg;
	result = Convert(srcImg.GetImages(), srcImg.GetImageCount(),
		srcImg.GetMetadata(),
		DXGI_FORMAT_R16G16B16A16_FLOAT, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT,
		scratchImg);

	if (SUCCEEDED(result)) {
		metadata = scratchImg.GetMetadata();
	}

	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;


	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresdesc.MipLevels = (UINT16)metadata.mipLevels;
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

		GetInstance().texBuffs[GetInstance().nextTexIndex]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
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
			map[tryNum == 0 ? key : key + std::to_string(tryNum)].meta = metadata;
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
}

TextureKey SpTextureManager::CreateDummyTexture(float width, float height, TextureKey key, bool initAsRenderTarget, bool useRatio)
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
	CD3DX12_HEAP_PROPERTIES texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC textureResourceDesc;
	
	if (useRatio)
	{
		Float2 ratio = { width, height };
		textureResourceDesc =
			CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R16G16B16A16_FLOAT, (UINT)(ratio.x * GetSpWindow()->width), (UINT)(ratio.y * GetSpWindow()->height), 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	}
	else {
		textureResourceDesc =
			CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R16G16B16A16_FLOAT, (UINT)width, (UINT)height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	}

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R16G16B16A16_FLOAT, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	GetInstance().texBuffs[GetInstance().nextTexIndex]->SetName(L"TEXTURE_BUFFER");

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

	if (useRatio)
	{
		SpTextureManager::GetInstance().texDataMap.Access(
			[&](auto& map) {
				TexData& pTexData = map[key];
				pTexData.meta = TexMetadata{};
				pTexData.meta.width = (size_t)(width * GetSpWindow()->width);
				pTexData.meta.height = (size_t)(height * GetSpWindow()->height);

				pTexData.ratio = {width, height};
			}
		);
	}
	else {
		SpTextureManager::GetInstance().texDataMap.Access(
			[&](auto& map) {
				TexMetadata& pTexMeta = map[key].meta;
		pTexMeta = TexMetadata{};
		pTexMeta.width = (size_t)width;
		pTexMeta.height = (size_t)height;
			}
		);
	}

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
	CD3DX12_HEAP_PROPERTIES texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R16G16B16A16_FLOAT, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R16G16B16A16_FLOAT, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex]));

	GetInstance().texBuffs[GetInstance().nextTexIndex]->SetName(L"TEXTURE_BUFFER");
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
			TexMetadata& pTexMeta = map[key].meta;
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
}

void SpTextureManager::LoadDiv(string filePath, int widthPer, int heightPer, int qx, int qy, const vector<TextureKey>& keys)
{
	auto& itr = keys.begin();
	int end = 0;
	for (int x = 0; x < qx; x++)
	{
		for (int y = 0; y < qy; y++)
		{
			if (end)
			{
				LoadSingleDiv(filePath, widthPer * x, heightPer * y, widthPer, heightPer, *itr + to_string(end));
				end++;
			}
			else {
				LoadSingleDiv(filePath, widthPer * x, heightPer * y, widthPer, heightPer, *itr);
				itr++;
				if (itr == keys.end()) {
					end = 1;
					itr--;
				}
			}
		}
	}
}

TextureKey SpTextureManager::LoadSingleDiv(string filePath, int originX, int originY, int width, int height, TextureKey key)
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
	ScratchImage srcImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, srcImg);

	ScratchImage trimed{};

	trimed.Initialize2D(srcImg.GetMetadata().format, width, height, srcImg.GetMetadata().arraySize, srcImg.GetMetadata().mipLevels);
	trimed.GetImage(0,0,0)->pixels[0] = srcImg.GetImage(0,0,0)->pixels[0];

	//size_t srcWidth = srcImg.GetMetadata().width;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			trimed.GetImage(0, 0, 0)->pixels[x * 4 + y * trimed.GetImage(0,0,0)->rowPitch] =
				srcImg.GetImage(0, 0, 0)->pixels[(x + originX) * 4 + (y + originY) * srcImg.GetImage(0,0,0)->rowPitch];

			trimed.GetImage(0, 0, 0)->pixels[x * 4 + y * trimed.GetImage(0, 0, 0)->rowPitch + 1] =
				srcImg.GetImage(0, 0, 0)->pixels[(x + originX) * 4 + (y + originY) * srcImg.GetImage(0, 0, 0)->rowPitch + 1];

			trimed.GetImage(0, 0, 0)->pixels[x * 4 + y * trimed.GetImage(0, 0, 0)->rowPitch + 2] =
				srcImg.GetImage(0, 0, 0)->pixels[(x + originX) * 4 + (y + originY) * srcImg.GetImage(0, 0, 0)->rowPitch + 2];

			trimed.GetImage(0, 0, 0)->pixels[x * 4 + y * trimed.GetImage(0, 0, 0)->rowPitch + 3] =
				srcImg.GetImage(0, 0, 0)->pixels[(x + originX) * 4 + (y + originY) * srcImg.GetImage(0, 0, 0)->rowPitch + 3];
		}
	}
	srcImg = move(trimed);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(srcImg.GetImages(), srcImg.GetImageCount(), srcImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		srcImg = move(mipChain);
		metadata = srcImg.GetMetadata();
	}

	//metadata.format = MakeSRGB(metadata.format);
	ScratchImage scratchImg;
	result = Convert(srcImg.GetImages(), srcImg.GetImageCount(),
		srcImg.GetMetadata(),
		DXGI_FORMAT_R16G16B16A16_FLOAT, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT,
		scratchImg);

	if (SUCCEEDED(result)) {
		metadata = scratchImg.GetMetadata();
	}

	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresdesc.MipLevels = (UINT16)metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	HRESULT hr = GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ins.texBuffs[ins.nextTexIndex]));

	if (hr != S_OK)
	{
		return string("notexture");
	};

	ins.texBuffs[ins.nextTexIndex]->SetName(L"TEXTURE_BUFFER");

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		ins.texBuffs[ins.nextTexIndex]->WriteToSubresource(
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
	heapHandle = ins.srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(ins.texBuffs[ins.nextTexIndex].Get(), &srvDesc, heapHandle);

	ins.textureMap.Access(
		[&](auto& map) {
			map[key] = ins.nextTexIndex;
		}
	);

	ins.texDataMap.Access(
		[&](auto& map) {
			map[key].meta = metadata;
		}
	);

	ins.isOccupied[ins.nextTexIndex] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!ins.isOccupied[i])
		{
			ins.nextTexIndex = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
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
			TexMetadata& pTexMeta = map[key].meta;
			pTexMeta = TexMetadata{};
			pTexMeta.width = 0;
			pTexMeta.height = 0;
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
}

void SpTextureManager::ResizeScreenTextures()
{
	unordered_map<TextureKey, TexData> resizing;
	GetInstance().texDataMap.Access(
		[&](auto& map) {
			for (auto& c : map)
			{
				if (c.second.ratio.x != 0)
				{
					resizing.insert(c);
				}
			}
		}
	);

	for (auto& r : resizing)
	{
		Release(r.first);
		CreateDummyTexture(r.second.ratio.x, r.second.ratio.y, r.first, true, true);
		GetWDX()->dev->CreateRenderTargetView(SpTextureManager::GetTextureBuff(r.first), nullptr,
			RTVManager::GetHeapCPUHandle((int)SpTextureManager::GetIndex(r.first)));
	}
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
			meta = map[key].meta;
		}
	);
	return meta;
}

SpTextureManager::TexData SpTextureManager::GetTextureData(TextureKey key)
{
	TexData data = {};
	GetInstance().texDataMap.Access(
		[&](auto& map) {
			data = map[key];
		}
	);
	return data;
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
			else ret = (int)itr->second;
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