#include "RTVManager.h"
#include "SpDirectX.h"
#include "SpSwapChainManager.h"
#include "SpDepth.h"

void RTVManager::SetRenderTargetToBackBuffer(UINT bbIndex)
{
	CloseCurrentResBar();
	GetSpDX()->cmdList->ClearDepthStencilView(GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
	SpDirectX* dx = GetSpDX();
	//リソースバリアーを書き込み可能状態に
	dx->barrierDesc.Transition.pResource = GetSCM()->backBuffers[bbIndex].Get();
	dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

	GetInstance().isAllResBarClosed = false;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetInstance().GetHeapCPUHandle(GetInstance().numRT - 2),
		bbIndex, GetSpDX()->dev->GetDescriptorHandleIncrementSize(GetInstance().heapDesc_.Type));
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	GetSpDX()->cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	GetInstance().currentRTIndex_[0] = GetInstance().numRT - 2 + bbIndex;

	for (int32_t i = 1; i < 8; i++)
	{
		GetInstance().currentRTIndex_[i] = -1;
	}
}

void RTVManager::SetRenderTargetToTexture(const TextureKey& key, bool clear)
{
	CloseCurrentResBar();
	GetSpDX()->cmdList->ClearDepthStencilView(GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
	int32_t index = (int32_t)SpTextureManager::GetIndex(key);

	SpDirectX* dx = GetSpDX();
	//リソースバリアーを書き込み可能状態に
	dx->barrierDesc.Transition.pResource = SpTextureManager::GetTextureBuff(key);
	dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

	GetInstance().isAllResBarClosed = false;

	//TODO:専用のDSVを用意
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();

	auto cpuhnd = (GetInstance().GetHeapCPUHandle(index));
	D3D12_CPU_DESCRIPTOR_HANDLE* pcpuhnd = &cpuhnd;
	GetSpDX()->cmdList->OMSetRenderTargets(1, pcpuhnd, false, &dsvH);

	GetInstance().currentRTIndex_[0] = index;
	for (int32_t i = 1; i < 8; i++)
	{
		GetInstance().currentRTIndex_[i] = -1;
	}

	if (clear)ClearCurrentRenderTarget({ 0, 0, 0, 0 });
}

void RTVManager::SetRenderTargets(const std::vector<TextureKey>& keys)
{
	CloseCurrentResBar();
	GetSpDX()->cmdList->ClearDepthStencilView(GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);

	for (auto& key : keys)
	{
		if (key == "CurrentBuffer")
		{
			SpDirectX* dx = GetSpDX();
			//リソースバリアーを書き込み可能状態に
			dx->barrierDesc.Transition.pResource = GetSCM()->backBuffers[GetSCM()->swapchain->GetCurrentBackBufferIndex()].Get();
			dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);
		}

		else
		{
			SpDirectX* dx = GetSpDX();
			//リソースバリアーを書き込み可能状態に
			dx->barrierDesc.Transition.pResource = SpTextureManager::GetTextureBuff(key);
			dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

			GetInstance().isAllResBarClosed = false;
		}
	}

	//TODO:専用のDSVを用意
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetSpDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> pcpuhnds;
	for (auto& key : keys)
	{
		if (key == "CurrentBuffer")
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetInstance().GetHeapCPUHandle(GetInstance().numRT - 2),
				GetSCM()->swapchain->GetCurrentBackBufferIndex(), GetSpDX()->dev->GetDescriptorHandleIncrementSize(GetInstance().heapDesc_.Type));
			pcpuhnds.push_back(rtvH);
		}
		else
		{
			auto cpuhnd = (GetInstance().GetHeapCPUHandle(SpTextureManager::GetIndex(key)));
			pcpuhnds.push_back(cpuhnd);
		}
	}

	GetSpDX()->cmdList->OMSetRenderTargets((UINT)keys.size(), &pcpuhnds.front(), false, &dsvH);

	for (int32_t i = 0; i < 8; i++)
	{
		if (i >= keys.size())
		{
			GetInstance().currentRTIndex_[i] = -1;
			continue;
		}
		GetInstance().currentRTIndex_[i] = SpTextureManager::GetIndex(keys.at(i));
	}
}

void RTVManager::SetRenderTargetToCurrentBB()
{
	SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void RTVManager::CreateRenderTargetTexture(int32_t width, int32_t height, const TextureKey& key)
{
	CreateRenderTargetTexture((float)width, (float)height, key, false);
}

void RTVManager::CreateRenderTargetTexture(float width, float height, const TextureKey& key, bool useScreenRatio)
{
	SpTextureManager::CreateDummyTexture(width, height, key, true, useScreenRatio);

	if (SpTextureManager::GetIndex(key) > GetInstance().numRT - 3) { throw "Its Gonna Eat Back Buffer memory"; return; }

	GetSpDX()->dev->CreateRenderTargetView(SpTextureManager::GetTextureBuff(key), nullptr,
		GetHeapCPUHandle((int32_t)SpTextureManager::GetIndex(key)));

	//デフォルトのリソースバリアをセット
	//ID3D12Resource* lastRes = GetWDX()->barrierDesc.Transition.pResource;
	//GetWDX()->barrierDesc.Transition.pResource = SpTextureManager::GetTextureBuff(key);
	//GetWDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//GetWDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//GetWDX()->cmdList->ResourceBarrier(1, &GetWDX()->barrierDesc);

	//GetWDX()->barrierDesc.Transition.pResource = lastRes;
}

void RTVManager::CreateHeaps()
{
	GetInstance().heapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	GetInstance().heapDesc_.NumDescriptors = GetInstance().numRT;
	GetSpDX()->dev->CreateDescriptorHeap(&GetInstance().heapDesc_, IID_PPV_ARGS(GetInstance().rtvHeaps_.GetAddressOf()));
}

int32_t RTVManager::GetCurrentRenderTarget()
{
	return GetInstance().currentRTIndex_[0];
}

void RTVManager::ClearCurrentRenderTarget(const Float4& color)
{
	float colour[] = { color.x, color.y, color.z, color.w };
	for (int32_t i = 0; i < 8; i++)
	{
		int32_t index = GetInstance().currentRTIndex_[i];
		if (index < 0) continue;

		GetSpDX()->cmdList->ClearRenderTargetView(GetHeapCPUHandle(index), colour, 0, nullptr);
	}
}

RTVManager& RTVManager::GetInstance()
{
	static RTVManager obj;
	return obj;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetHeapCPUHandle(int32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = GetInstance().rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetSpDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
	return heapHandle;
}

void RTVManager::CloseCurrentResBar()
{
	for (int32_t i = 0; i < 8; i++)
	{
		int32_t index = GetInstance().currentRTIndex_[i];
		if (index < 0) continue;

		if (GetInstance().isAllResBarClosed)
		{
			continue;
		}

		if (index >= GetInstance().numRT - 2)
		{
			//リソースバリアーを戻す
			GetSpDX()->barrierDesc.Transition.pResource = GetSCM()->backBuffers[index - GetInstance().numRT + 2].Get();
			GetSpDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			GetSpDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

			GetSpDX()->cmdList->ResourceBarrier(1, &GetSpDX()->barrierDesc);
			continue;
		}

		//リソースバリアーを戻す
		GetSpDX()->barrierDesc.Transition.pResource = SpTextureManager::GetInstance().texBuffs[index].Get();
		GetSpDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		GetSpDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		GetSpDX()->cmdList->ResourceBarrier(1, &GetSpDX()->barrierDesc);
	}

}
