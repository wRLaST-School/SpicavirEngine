#include "wTexture.h"
#include "wDirectX.h"

wTexture wtex;

void wTexture::LoadTexture(string path)
{
	//画像データの作成
	const int texWidth = 256;
	const int imageDataCount = texWidth * texWidth;

	unique_ptr<XMFLOAT4> imageData = unique_ptr<XMFLOAT4>(new XMFLOAT4[imageDataCount]);

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	wstring wstrpath = wstring(path.begin(), path.end());
	const wchar_t* wpath = wstrpath.c_str();

	LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};

	HRESULT result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);
	
	//テクスチャバッファ
	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	
	texresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresdesc.Format = metadata.format;
	texresdesc.Width = metadata.width;
	texresdesc.Height = (UINT)metadata.height;
	texresdesc.DepthOrArraySize = metadata.height;
	texresdesc.MipLevels = metadata.mipLevels;
	texresdesc.SampleDesc.Count = 1;

	GetWDX()->dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		HRESULT hr = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(hr));
	}
}

void InitTextureBuff()
{
	wtex.LoadTexture("Resources/think.png");
}

wTexture* GetTextureManager()
{
	return &wtex;
}
