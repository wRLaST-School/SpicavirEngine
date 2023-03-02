#include "RTVManager.h"
#include "SpDirectX.h"
#include "SpSwapChainManager.h"
#include "SpDepth.h"

void RTVManager::SetRenderTargetToBackBuffer(UINT bbIndex)
{
	CloseCurrentResBar(GetCurrentRenderTarget());
	GetWDX()->cmdList->ClearDepthStencilView(GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
	SpDirectX* dx = GetWDX();
	//���\�[�X�o���A�[���������݉\��Ԃ�
	dx->barrierDesc.Transition.pResource = GetSCM()->backBuffers[bbIndex].Get();
	dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

	GetInstance().isAllResBarClosed = false;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetInstance().GetHeapCPUHandle(GetInstance().numRT - 2),
		bbIndex, GetWDX()->dev->GetDescriptorHandleIncrementSize(GetInstance().heapDesc.Type));
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	GetWDX()->cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	GetInstance().currentRTIndex = GetInstance().numRT - 2 + bbIndex;
}

void RTVManager::SetRenderTargetToTexture(TextureKey key, bool clear)
{
	CloseCurrentResBar(GetCurrentRenderTarget());
	GetWDX()->cmdList->ClearDepthStencilView(GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
	int index = (int)SpTextureManager::GetIndex(key);

	SpDirectX* dx = GetWDX();
	//���\�[�X�o���A�[���������݉\��Ԃ�
	dx->barrierDesc.Transition.pResource = SpTextureManager::GetTextureBuff(key);
	dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

	GetInstance().isAllResBarClosed = false;

	//TODO:��p��DSV��p��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();

	auto cpuhnd = (GetInstance().GetHeapCPUHandle(index));
	D3D12_CPU_DESCRIPTOR_HANDLE* pcpuhnd = &cpuhnd;
	GetWDX()->cmdList->OMSetRenderTargets(1, pcpuhnd, false, &dsvH);

	GetInstance().currentRTIndex = index;

	if(clear)ClearCurrentRenderTarget({0, 0, 0, 0});
}

void RTVManager::CreateRenderTargetTexture(int width, int height, TextureKey key)
{
	CreateRenderTargetTexture((float)width, (float)height, key, false);
}

void RTVManager::CreateRenderTargetTexture(float width, float height, TextureKey key, bool useScreenRatio)
{
	key = SpTextureManager::CreateDummyTexture(width, height, key, true, useScreenRatio);

	if (SpTextureManager::GetIndex(key) > GetInstance().numRT - 3) { throw "Its Gonna Eat Back Buffer memory"; return; }

	GetWDX()->dev->CreateRenderTargetView(SpTextureManager::GetTextureBuff(key), nullptr,
		GetHeapCPUHandle((int)SpTextureManager::GetIndex(key)));

	//�f�t�H���g�̃��\�[�X�o���A���Z�b�g
	//ID3D12Resource* lastRes = GetWDX()->barrierDesc.Transition.pResource;
	//GetWDX()->barrierDesc.Transition.pResource = SpTextureManager::GetTextureBuff(key);
	//GetWDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//GetWDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//GetWDX()->cmdList->ResourceBarrier(1, &GetWDX()->barrierDesc);

	//GetWDX()->barrierDesc.Transition.pResource = lastRes;
}

void RTVManager::CreateHeaps()
{
	GetInstance().heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	GetInstance().heapDesc.NumDescriptors = GetInstance().numRT;
	GetWDX()->dev->CreateDescriptorHeap(&GetInstance().heapDesc, IID_PPV_ARGS(GetInstance().rtvHeaps.GetAddressOf()));
}

int RTVManager::GetCurrentRenderTarget()
{
	return GetInstance().currentRTIndex;
}

void RTVManager::ClearCurrentRenderTarget(Float4 color)
{
	float colour[] = {color.x, color.y, color.z, color.w};
	GetWDX()->cmdList->ClearRenderTargetView(GetHeapCPUHandle(GetCurrentRenderTarget()), colour, 0, nullptr);
}

RTVManager& RTVManager::GetInstance()
{
	static RTVManager obj;
	return obj;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetHeapCPUHandle(int index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	heapHandle = GetInstance().rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	heapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
	return heapHandle;
}

void RTVManager::CloseCurrentResBar(int index)
{
	if (index < 0 ) return;

	if (GetInstance().isAllResBarClosed)
	{
		return;
	}

	if (index >= GetInstance().numRT - 2)
	{
		//���\�[�X�o���A�[��߂�
		GetWDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		GetWDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		GetWDX()->cmdList->ResourceBarrier(1, &GetWDX()->barrierDesc);
		return;
	}

	//���\�[�X�o���A�[��߂�
	GetWDX()->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	GetWDX()->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	GetWDX()->cmdList->ResourceBarrier(1, &GetWDX()->barrierDesc);

}
