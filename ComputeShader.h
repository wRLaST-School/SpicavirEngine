#pragma once
#include <StructuredBuffer.h>

template <class Object>
class ComputeShader
{
public:
	ComputeShader(std::wstring shaderFile) {};
	~ComputeShader() {};

	void Execute() {

	};

	StructuredBuffer<Object> buf;
	Object* data;

private:
	void CreateRS(std::wstring shaderFile);
	void CreatePL();

	void CreateFence();

	void CreateHeap();
	void Map();

	void CreateCommandList();

	void Init();


	Microsoft::WRL::ComPtr<ID3D12RootSignature> rs;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pl;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
	Microsoft::WRL::ComPtr<ID3DBlob> shader;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAlloc;
	Microsoft::WRL::ComPtr<ID3D12CommandList> cmdList;

};

template<class Object>
inline void ComputeShader<Object>::CreateRS(std::wstring shaderFile)
{
	//繧ｷ繧ｧ繝ｼ繝�繝ｼ繧ｳ繝ｳ繝代う繝ｫ
	auto hr = D3DCompileFromFile(shaderFile, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_1", D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	//繧ｷ繧ｧ繝ｼ繝�繝ｼ縺九ｉ繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣諠��ｱ繧貞叙蠕�
	ID3DBlob* sig = nullptr;
	hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(),
		D3D_BLOB_ROOT_SIGNATURE, 0, &sig);

	assert(SUCCEEDED(hr));

	//繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣縺ｮ逕滓��
	hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
		IID_PPV_ARGS(&rs));

	assert(SUCCEEDED(hr));
}

template<class Object>
inline void ComputeShader<Object>::CreatePL()
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rs;

	auto hr = dev->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pl));
	assert(SUCCEEDED(hr));
}

template<class Object>
inline void ComputeShader<Object>::CreateHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	assert(SUCCEEDED(hr));
}

template<class Object>
inline void ComputeShader<Object>::Map()
{
	data = buf.contents;
}

template<class Object>
inline void ComputeShader<Object>::CreateCommandList()
{
	auto hr = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
		IID_PPV_ARGS(&allo));
	hr = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, allo, nullptr,
		IID_PPV_ARGS(&list));
	return hr;
}
