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
	//シェーダーコンパイル
	auto hr = D3DCompileFromFile(shaderFile, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_1", D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	//シェーダーからルートシグネチャ情報を取得
	ID3DBlob* sig = nullptr;
	hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(),
		D3D_BLOB_ROOT_SIGNATURE, 0, &sig);

	assert(SUCCEEDED(hr));

	//ルートシグネチャの生成
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
