
#pragma once
#include <SpDirectX.h>

template <class Contents>
class StructuredBuffer {
public:
	StructuredBuffer()
	{
		Init();
	}

	~StructuredBuffer()
	{
		//アンマップ
		CD3DX12_RANGE readRange(0, 0);
		if (buf_) {
			buf_->Unmap(0, &readRange);
			buf_->Release();
		}
	};

	void Init();

	void RegisterUAV(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);

	bool HasInited() const
	{
		return hasInited_;
	}

	Contents* contents;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> buf_ = nullptr;
	bool hasInited_ = false;
};

template<class Contents>
inline void StructuredBuffer<Contents>::Init()
{
	auto dev = GetSpDX()->dev;
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(m_sizeOfElement * m_numElement);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	int32_t bufferNo = 0;

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;

	dev->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&buf_)
	);

	//マップ
	CD3DX12_RANGE readRange(0, 0);
	buf_->Map(0, &readRange, reinterpret_cast<void**>(&contents));

	hasInited_ = true;
}

template<class Contents>
inline void StructuredBuffer<Contents>::RegisterUAV(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
{
	if (!hasInited_) {
		return;
	}

	auto dev = GetSpDX()->dev;;

	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements = static_cast<UINT>(m_numElement);
	desc.Buffer.StructureByteStride = m_sizeOfElement;
	dev->CreateUnorderedAccessView(
		buf_.Get(),
		nullptr,
		&desc,
		descriptorHandle
	);
}
