#include "SpTextureManager.h"
#include "SpDirectX.h"
#include <RTVManager.h>

#define SPTEX_NOTEXTURE_FOUND_ -1024

void SpTextureManager::Create()
{
	GetInstance().nextTexIndex_ = 0;
	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = wMaxSRVCount;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&GetInstance().srvHeap));
}

void SpTextureManager::Init()
{
	SpTextureManager& ins = GetInstance();
	ins.nextTexIndex_ = 0;

	for (int32_t i = 0; i < wMaxSRVCount; i++)
	{
		ins.isOccupied_[i] = false;
	}

	ins.textureMap_.Access([&](auto& map) {
		map.clear();
		});

	for (ComPtr<ID3D12Resource>& texbuffs : ins.texBuffs)
	{
		texbuffs = nullptr;
	}

	SpTextureManager::LoadTexture("Resources/notexture.png", "notexture");
	AddMasterTextureKey("notexture");
}

TextureKey SpTextureManager::LoadTexture(const string& filePath, const TextureKey& key)
{
	sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
	bool alreadyRegistered = false;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});
	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exists. skipping.") + string("\n")).c_str());
		return key;
	}
	OutputDebugStringA((string("Loading : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex_) + string(")\n")).c_str());
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

	//�e�N�X�`���o�b�t�@
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
	if (!texresdesc.Height || !texresdesc.Width || !texresdesc.DepthOrArraySize)
	{
		OutputDebugStringW(L"Texture Size or Depth Appeared to be Zero. Texture was not Loaded.\n");
		return "notexture";
	}
	HRESULT hr = GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&ins.texBuffs[ins.nextTexIndex_]));

	if (hr != S_OK)
	{
		return string("notexture");
	};

	ins.texBuffs[ins.nextTexIndex_]->SetName(L"TEXTURE_BUFFER");

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		ins.texBuffs[ins.nextTexIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}

	//�V�F�[�_�[���\�[�X�r���[�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = ins.srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(ins.texBuffs[ins.nextTexIndex_].Get(), &srvDesc, heapHandle);

	ins.textureMap_.Access(
		[&](auto& map) {
			map[key] = ins.nextTexIndex_;
		}
	);

	ins.texDataMap_.Access(
		[&](auto& map) {
			map[key].meta = metadata;
		}
	);

	ins.isOccupied_[ins.nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!ins.isOccupied_[i])
		{
			ins.nextTexIndex_ = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
}

TextureKey SpTextureManager::LoadTextureWithUniqueKey(const string& filePath, const TextureKey& key)
{
	OutputDebugStringA((string("Loading Unique : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex_) + string(")\n")).c_str());
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
		DXGI_FORMAT_R8G8B8A8_UNORM, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT,
		scratchImg);

	if (SUCCEEDED(result)) {
		metadata = scratchImg.GetMetadata();
	}

	//�e�N�X�`���o�b�t�@
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

	if (!texresdesc.Height || !texresdesc.Width || !texresdesc.DepthOrArraySize)
	{
		OutputDebugStringW(L"Texture Size or Depth Appeared to be Zero. Texture was not Loaded.\n");
		return "notexture";
	}
	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex_]));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		GetInstance().texBuffs[GetInstance().nextTexIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
	}

	//�V�F�[�_�[���\�[�X�r���[�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex_].Get(), &srvDesc, heapHandle);

	int32_t tryNum = 0;
	bool succeeded = false;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		string tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		SpTextureManager::GetInstance().textureMap_.Access(
			[&](auto& map) {
				succeeded = map.try_emplace(tryKey, SpTextureManager::GetInstance().nextTexIndex_).second;
			}
		);
	}

	ins.textureMap_.Access(
		[&](auto& map) {
			map[tryNum == 0 ? key : key + std::to_string(tryNum)] = ins.nextTexIndex_;
		}
	);

	ins.texDataMap_.Access(
		[&](auto& map) {
			map[tryNum == 0 ? key : key + std::to_string(tryNum)].meta = metadata;
		}
	);

	GetInstance().isOccupied_[GetInstance().nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i])
		{
			GetInstance().nextTexIndex_ = i;
			sPerSceneTextures[sCurrentSceneResIndex].push_back(tryNum == 0 ? key : key + std::to_string(tryNum));
			return tryNum == 0 ? key : key + std::to_string(tryNum);
		}
	}

	throw std::out_of_range("out of texture resource");
}

TextureKey SpTextureManager::CreateDummyTexture(float width, float height, const TextureKey& key, bool initAsRenderTarget, bool useRatio)
{
	sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i] && i <= 253)
		{
			GetInstance().nextTexIndex_ = i;
			sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
			break;
		}
	}

	bool alreadyRegistered = false;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});

	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exist. skipping.") + string("\n")).c_str());
		return key;
	}

	OutputDebugStringA((string("Creating Dummy Tex U: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex_) + string("\n")).c_str());
	CD3DX12_HEAP_PROPERTIES texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC textureResourceDesc;
	
	if (useRatio)
	{
		Float2 ratio = { width, height };
		textureResourceDesc =
			CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)(ratio.x * GetSpWindow()->width), (UINT)(ratio.y * GetSpWindow()->height), 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	}
	else {
		textureResourceDesc =
			CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)width, (UINT)height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	}

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex_]));

	GetInstance().texBuffs[GetInstance().nextTexIndex_]->SetName(L"TEXTURE_BUFFER");

	//�V�F�[�_�[���\�[�X�r���[�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex_].Get(), &srvDesc, heapHandle);

	SpTextureManager::GetInstance().textureMap_.Access(
		[&](auto& map) {
			map[key] = SpTextureManager::GetInstance().nextTexIndex_;
		}
	);

	if (useRatio)
	{
		SpTextureManager::GetInstance().texDataMap_.Access(
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
		SpTextureManager::GetInstance().texDataMap_.Access(
			[&](auto& map) {
				TexMetadata& pTexMeta = map[key].meta;
		pTexMeta = TexMetadata{};
		pTexMeta.width = (size_t)width;
		pTexMeta.height = (size_t)height;
			}
		);
	}

	GetInstance().isOccupied_[GetInstance().nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i])
		{
			GetInstance().nextTexIndex_ = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
}

TextureKey SpTextureManager::CreateDummyTextureWithUniqueKey(int32_t width, int32_t height, const TextureKey& key, bool initAsRenderTarget)
{//�e�N�X�`���o�b�t�@
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i] && i <= 253)
		{
			GetInstance().nextTexIndex_ = i;
			sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
			break;
		}
	}
	OutputDebugStringA((string("Creating Dummy Tex U: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex_) + string("\n")).c_str());
	CD3DX12_HEAP_PROPERTIES texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0, 0, 0, 0} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		initAsRenderTarget ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_GENERIC_READ,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextTexIndex_]));

	GetInstance().texBuffs[GetInstance().nextTexIndex_]->SetName(L"TEXTURE_BUFFER");
	//�V�F�[�_�[���\�[�X�r���[�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetInstance().texBuffs[GetInstance().nextTexIndex_].Get(), &srvDesc, heapHandle);

	int32_t tryNum = 0;
	bool succeeded = false;
	string tryKey;
	for (tryNum = 0; !succeeded; tryNum++)
	{
		tryKey = tryNum == 0 ? key : key + std::to_string(tryNum);
		SpTextureManager::GetInstance().textureMap_.Access(
			[&](auto& map) {
				succeeded = map.try_emplace(tryKey, SpTextureManager::GetInstance().nextTexIndex_).second;
			}
		);
	}
	SpTextureManager::GetInstance().texDataMap_.Access(
		[&](auto& map) {
			TexMetadata& pTexMeta = map[key].meta;
			pTexMeta = TexMetadata{};
			pTexMeta.width = width;
			pTexMeta.height = height;
		}
	);

	GetInstance().isOccupied_[GetInstance().nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i])
		{
			GetInstance().nextTexIndex_ = i;
			sPerSceneTextures[sCurrentSceneResIndex].push_back(tryKey);
			return tryKey;
		}
	}

	throw std::out_of_range("out of texture resource");
}

void SpTextureManager::LoadDiv(const string& filePath, int32_t widthPer, int32_t heightPer, int32_t qx, int32_t qy, const vector<TextureKey>& keys)
{
	auto itr = keys.begin();
	int32_t end = 0;
	for (int32_t x = 0; x < qx; x++)
	{
		for (int32_t y = 0; y < qy; y++)
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

TextureKey SpTextureManager::LoadSingleDiv(string filePath, int32_t originX, int32_t originY, int32_t width, int32_t height, const TextureKey& key)
{
	sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
	bool alreadyRegistered = false;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});
	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exists. skipping.") + string("\n")).c_str());
		return key;
	}
	OutputDebugStringA((string("Loading : ") + key + string(" (Heap Index : ") + to_string(GetInstance().nextTexIndex_) + string(")\n")).c_str());
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
	for (int32_t x = 0; x < width; x++) {
		for (int32_t y = 0; y < height; y++) {
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

	//�e�N�X�`���o�b�t�@
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
		IID_PPV_ARGS(&ins.texBuffs[ins.nextTexIndex_]));

	if (hr != S_OK)
	{
		return string("notexture");
	};

	ins.texBuffs[ins.nextTexIndex_]->SetName(L"TEXTURE_BUFFER");

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		ins.texBuffs[ins.nextTexIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}

	//�V�F�[�_�[���\�[�X�r���[�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = ins.srvHeap->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * SpTextureManager::GetInstance().nextTexIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(ins.texBuffs[ins.nextTexIndex_].Get(), &srvDesc, heapHandle);

	ins.textureMap_.Access(
		[&](auto& map) {
			map[key] = ins.nextTexIndex_;
		}
	);

	ins.texDataMap_.Access(
		[&](auto& map) {
			map[key].meta = metadata;
		}
	);

	ins.isOccupied_[ins.nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!ins.isOccupied_[i])
		{
			ins.nextTexIndex_ = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
}

TextureKey SpTextureManager::CreatePlainSRV(const TextureKey& key)
{
	sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i] && i <= 253)
		{
			GetInstance().nextTexIndex_ = i;
			sPerSceneTextures[sCurrentSceneResIndex].push_back(key);
			break;
		}
	}

	bool alreadyRegistered = false;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			if (map.count(key) != 0) alreadyRegistered = true;
		});

	if (alreadyRegistered)
	{
		OutputDebugStringA((string("Texture : ") + key + string(" already exist. skipping.") + string("\n")).c_str());
		return key;
	}

	OutputDebugStringA((string("Creating Plain SRV: ") + key + string(" on ") + to_string(GetInstance().nextTexIndex_) + string("\n")).c_str());

	SpTextureManager::GetInstance().textureMap_.Access(
		[&](auto& map) {
			map[key] = SpTextureManager::GetInstance().nextTexIndex_;
		}
	);

	SpTextureManager::GetInstance().texDataMap_.Access(
		[&](auto& map) {
			TexMetadata& pTexMeta = map[key].meta;
			pTexMeta = TexMetadata{};
			pTexMeta.width = 0;
			pTexMeta.height = 0;
		}
	);

	GetInstance().isOccupied_[GetInstance().nextTexIndex_] = true;

	for (size_t i = 0; i < wMaxSRVCount; i++)
	{
		if (!GetInstance().isOccupied_[i])
		{
			GetInstance().nextTexIndex_ = i;
			return key;
		}
	}

	throw std::out_of_range("out of texture resource");
}

void SpTextureManager::ResizeScreenTextures()
{
	unordered_map<TextureKey, TexData> resizing;
	GetInstance().texDataMap_.Access(
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
			RTVManager::GetHeapCPUHandle((int32_t)SpTextureManager::GetIndex(r.first)));
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE SpTextureManager::GetCPUDescHandle(const TextureKey& key)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetCPUDescriptorHandleForHeapStart();
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * map[key];

		}
	);
	return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SpTextureManager::GetGPUDescHandle(const TextureKey& key)
{
	D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = SpTextureManager::GetInstance().srvHeap->GetGPUDescriptorHandleForHeapStart();
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * map[key];

		}
	);
	return heapHandle;
}

TexMetadata SpTextureManager::GetTextureMetadata(const TextureKey& key)
{
	TexMetadata meta;
	GetInstance().texDataMap_.Access(
		[&](auto& map) {
			meta = map[key].meta;
		}
	);
	return meta;
}

SpTextureManager::TexData SpTextureManager::GetTextureData(const TextureKey& key)
{
	TexData data = {};
	GetInstance().texDataMap_.Access(
		[&](auto& map) {
			data = map[key];
		}
	);
	return data;
}

ID3D12Resource* SpTextureManager::GetTextureBuff(const TextureKey& key)
{
	SRVHeapIndex index = 114514;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			index = map[key];
		}
	);
	return SpTextureManager::GetInstance().texBuffs[index].Get();
}

int32_t SpTextureManager::GetIndex(const TextureKey& key)
{
	int32_t ret = 0;
	GetInstance().textureMap_.Access(
		[&](auto& map) {
			auto itr = map.find(key);
			if (itr == map.end()) ret = SPTEX_NOTEXTURE_FOUND_;
			else ret = (int32_t)itr->second;
		}
	);

	return ret;
}

void SpTextureManager::AddMasterTextureKey(const TextureKey& key)
{
	sMasterTextures.push_back(key);
}

void SpTextureManager::Release(const TextureKey& key)
{
	SpTextureManager& ins = GetInstance();

	OutputDebugStringA((string("Unloading : ") + key + string(" (Heap Index : ") + to_string(GetIndex(key)) + string("\n")).c_str());
	if (GetIndex(key) == SPTEX_NOTEXTURE_FOUND_) {
		OutputDebugStringA("No Texture Found.\n");
		return;
	}

	ins.isOccupied_[GetIndex(key)] = false;
	ins.textureMap_.Access(
		[&](auto& map) {
			ins.texBuffs[map[key]] = ComPtr<ID3D12Resource>();
			map.erase(key);
		}
	);

	ins.texDataMap_.Access(
		[&](auto& map) {
			map.erase(key);
		}
	);
	return;
}

void SpTextureManager::ReleasePerSceneTexture()
{
	int32_t lastSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
	for (auto itr = sPerSceneTextures[lastSceneResIndex].begin(); itr != sPerSceneTextures[lastSceneResIndex].end(); itr++)
	{
		bool usingInCurrentScene = false;
		//���̃V�[���ł��g���邩�`�F�b�N
		for (auto key : sPerSceneTextures[sCurrentSceneResIndex])
		{
			if (key == *itr)
			{
				usingInCurrentScene = true;
				break;
			}
		}

		//�}�X�^�[�e�N�X�`�����`�F�b�N
		for (auto& tk : sMasterTextures)
		{
			if (*itr == tk)
			{
				usingInCurrentScene = true;
			}
		}

		if (!usingInCurrentScene) //���̃V�[���Ŏg���Ă��Ȃ��Ȃ烊���[�X
		{
			Release(*itr);
		}
	}
	sPerSceneTextures[lastSceneResIndex].clear();
}

void SpTextureManager::PreLoadNewScene()
{
	sCurrentSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
}

SpTextureManager& SpTextureManager::GetInstance()
{
	static SpTextureManager obj;
	return obj;
}

list<TextureKey> SpTextureManager::sPerSceneTextures[2] = {};
int32_t SpTextureManager::sCurrentSceneResIndex = 0;

list<TextureKey> SpTextureManager::sMasterTextures;