#include "wTextureManager.h"
#include "wDirectX.h"
#include "wTexture.h"

wTextureManager wtm;

void wTextureManager::Init()
{
	//設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;

	//生成
	GetWDX()->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&srvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	const size_t wMaxSRVCount = 256;

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = wMaxSRVCount;

	GetWDX()->dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの生成
	basicHeapHandle.ptr += GetWDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = GetTexture()->texresdesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = GetTexture()->texresdesc.MipLevels;

	GetWDX()->dev->CreateShaderResourceView(GetTexture()->texBuff.Get(), &srvDesc, srvHandle);
}

void CreateAndInitTextureManager()
{
	wtm = wTextureManager();
	wtm.Init();
}

wTextureManager* GetTextureManager()
{
	return &wtm;
}
