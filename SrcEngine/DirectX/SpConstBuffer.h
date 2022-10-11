#pragma once
#include "Essentials.h"
#include "SpDirectX.h"
template <class Contents> class SpConstBuffer
{
public:
	//ConstBufferを生成してマッピング
	SpConstBuffer() {
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbheapprop{};
		cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

		//リソース設定
		D3D12_RESOURCE_DESC cbresdesc{};
		cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbresdesc.Width = (sizeof(Contents) + 0xff) & ~0xff;
		cbresdesc.Height = 1;
		cbresdesc.DepthOrArraySize = 1;
		cbresdesc.MipLevels = 1;
		cbresdesc.SampleDesc.Count = 1;
		cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//GPUリソースの生成
		GetWDX()->dev->CreateCommittedResource(
			&cbheapprop,
			D3D12_HEAP_FLAG_NONE,
			&cbresdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer));

		buffer->Map(0, nullptr, (void**)&contents);
	};

	~SpConstBuffer() {
		buffer->Unmap(0, nullptr);
	};

	SpConstBuffer<Contents>& operator=(const SpConstBuffer<Contents>& second) {
		*this->contents = *second.contents;
		return *this;
	}
	
public:
	ComPtr<ID3D12Resource> buffer;
	Contents* contents;
};

namespace RootParamIndex {
	enum RootParamIndex_
	{
		Material,
		Texture,
		WMatrix,
		VPMatrix,
		Brightness,
		LightBasic,

		Max
	};
}