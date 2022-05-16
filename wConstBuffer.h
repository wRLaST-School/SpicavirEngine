#pragma once
#include "Essentials.h"
#include "wDirectX.h"
template <class Contents> class wConstBuffer
{
public:
	//ConstBufferを生成してマッピング
	wConstBuffer() {
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

	~wConstBuffer() {
		buffer->Unmap(0, nullptr);
	};

	void operator=(Contents c) {
		*contents = c;
	};
	
public:
	ComPtr<ID3D12Resource> buffer;
	//ComPtr<ID3D12DescriptorHeap> descHeap = nullptr;
	Contents* contents;
};
