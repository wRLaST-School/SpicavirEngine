#pragma once
#include <StructuredBuffer.h>
#include <Essentials.h>
#include <SpDirectX.h>

template <class Object>
class ComputeShader
{
public:
	ComputeShader(std::wstring shaderFile) {
		Init(shaderFile);
	};
	~ComputeShader() {};

	void Execute();

	StructuredBuffer<Object> buf;
	Object* data;

private:
	void CreateRS(std::wstring shaderFile);
	void CreatePL();

	void CreateFence();

	void CreateHeap();
	void RegisterUAV();
	void Map();

	void CreateCommandList();

	//Create系全部呼び出し
	void Init(std::wstring shaderFile);


	Microsoft::WRL::ComPtr<ID3D12RootSignature> rs;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pl;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
	Microsoft::WRL::ComPtr<ID3DBlob> shader;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	uint64_t fenceval = 0;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

};

template<class Object>
inline void ComputeShader<Object>::Execute()
{
	cmdAlloc->Reset();
	cmdList->Release(cmdAlloc, nullptr);

	cmdList->SetComputeRootSignature(rs);
	cmdList->SetPipelineState(pl);
	cmdList->SetDescriptorHeaps(1, &heap);

	auto gpuDescHandle = heap->GetGPUDescriptorHandleForHeapStart();

	cmdList->SetComputeRootDescriptorTable(0, gpuDescHandle);

	//実行
	cmdList->Dispatch(1, 1, 1);

	cmdList->Close();

	ID3D12CommandList* cmd[] = {
		cmdList->GetList(),
	};

	GetSpDX()->cmdQueue->ExecuteCommandLists(1, cmd);

	//描画コマンドが終わったら次のフレームの準備
	GetSpDX()->cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

template<class Object>
inline void ComputeShader<Object>::CreateRS(std::wstring shaderFile)
{

	//シェーダーコンパイル
	auto hr = D3DCompileFromFile(shaderFile, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_1", D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	ID3DBlob* sig = nullptr;
	hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(),
		D3D_BLOB_ROOT_SIGNATURE, 0, &sig);

	assert(SUCCEEDED(hr));

	//RS作成
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

	//PL作成
	auto hr = dev->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pl));
	assert(SUCCEEDED(hr));
}

template<class Object>
inline void ComputeShader<Object>::CreateFence()
{
	GetSpDX()->dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

template<class Object>
inline void ComputeShader<Object>::CreateHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//Heap作成
	auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	assert(SUCCEEDED(hr));
}

template<class Object>
inline void ComputeShader<Object>::RegisterUAV()
{
	buf.RegisterUAV(heap->GetCPUDescriptorHandleForHeapStart());
}

template<class Object>
inline void ComputeShader<Object>::Map()
{
	data = buf.contents;
}

template<class Object>
inline void ComputeShader<Object>::CreateCommandList()
{
	//コマンドアロケーターとリスト作成
	auto hr = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
		IID_PPV_ARGS(&allo));
	hr = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, allo, nullptr,
		IID_PPV_ARGS(&list));
	return hr;
}

template<class Object>
inline void ComputeShader<Object>::Init(std::wstring shaderFile)
{
	CreateRS(shaderFile);
	CreatePL();

	CreateFence();

	CreateHeap();
	RegisterUAV();

	Map();

	CreateCommandList();
}
