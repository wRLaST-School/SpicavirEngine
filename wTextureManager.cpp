#include "wTextureManager.h"
#include "wDirectX.h"

void wTextureManager::Create()
{
	GetInstance().nextRegisteredTextureIndex = 0;
	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	//index��unsigned char�ɂȂ��Ă邩�炱����ύX����Ȃ炻�������ύX
	const size_t wMaxSRVCount = 256;

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = wMaxSRVCount;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&GetInstance().srvHeap));
}

void wTextureManager::Init()
{
	GetInstance().nextRegisteredTextureIndex = 0;
	GetInstance().textureMap.clear();
	GetInstance().texBuffs.clear();

	wTextureManager::LoadTexture("Resources/notexture.png", "notexture");
}

TextureKey wTextureManager::LoadTexture(string filePath, TextureKey key)
{
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	GetInstance().texBuffs.emplace_back(nullptr);

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);

	//�e�N�X�`���o�b�t�@
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

	//�V�F�[�_�[���\�[�X�r���[�̐���
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

	wTextureManager::GetInstance().nextRegisteredTextureIndex++;

	return key;
}

TextureKey wTextureManager::LoadTextureWithUniqueKey(string filePath, TextureKey key)
{
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	D3D12_RESOURCE_DESC texresdesc{};

	GetInstance().texBuffs.emplace_back(nullptr);

	wstring wstrpath = wstring(filePath.begin(), filePath.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);

	//�e�N�X�`���o�b�t�@
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

	//�V�F�[�_�[���\�[�X�r���[�̐���
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

	wTextureManager::GetInstance().nextRegisteredTextureIndex++;

	return tryNum == 0 ? key : key + std::to_string(tryNum);
}

TextureKey wTextureManager::CreateDummyTexture(int width, int height, TextureKey key)
{
	D3D12_HEAP_PROPERTIES texHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC textureResourceDesc = 
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1, 1,0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	GetInstance().texBuffs.emplace_back(nullptr);

	D3D12_CLEAR_VALUE clval = { DXGI_FORMAT_R8G8B8A8_UNORM, {0.0f, 0.0f, 0.0f, 0.0f} };

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clval,
		IID_PPV_ARGS(&GetInstance().texBuffs[GetInstance().nextRegisteredTextureIndex]));

	//�V�F�[�_�[���\�[�X�r���[�̐���
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

	wTextureManager::GetInstance().nextRegisteredTextureIndex++;
	return key;
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

wTextureManager &wTextureManager::GetInstance()
{
	static wTextureManager obj;
	return obj;
}