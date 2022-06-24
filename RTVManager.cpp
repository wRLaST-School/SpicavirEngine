#include "RTVManager.h"
#include "wDirectX.h"
#include "wSwapChainManager.h"
#include "wDepth.h"

void RTVManager::SetRenderTargetToBackBuffer(UINT bbIndex)
{
	wDirectX* dx = GetWDX();
	//リソースバリアーを書き込み可能状態に
	dx->barrierDesc.Transition.pResource = GetSCM()->backBuffers[bbIndex].Get();
	dx->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	dx->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dx->cmdList->ResourceBarrier(1, &dx->barrierDesc);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetSCM()->rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex, GetWDX()->dev->GetDescriptorHandleIncrementSize(GetSCM()->heapDesc.Type));
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = GetWDepth()->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	GetWDX()->cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
}

RenderTarget* RTVManager::GetCurrentRenderTarget()
{
	return RTVManager::GetInstance().currentRT;
}

RTVManager& RTVManager::GetInstance()
{
	static RTVManager obj;
	return obj;
}
