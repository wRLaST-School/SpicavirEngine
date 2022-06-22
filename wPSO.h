#pragma once
#include "Essentials.h"
#include "wShader.h"
//TODO:ê›åvÇ™ÉKÉoÇ∑Ç¨ÇÈÇÃÇ≈pso,rootsignatureÇÃê›åvÇå©íºÇ∑
class wPSO
{
public:
	wPSO() {};
	wPSO(wShader* shader);
	void InitDesc();
	void InitDesc2D();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psod{};
	wShader* shader; 
	ComPtr<ID3D12PipelineState> pso = nullptr;
	void Create();
};

void RegisterAndInitPSO(string id, wShader* shader);
void RegisterAndInitPSOfor2D(string id, wShader* shader);
D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPSODesc(string id);
ID3D12PipelineState* GetPSO(string id);
wPSO* GetWPSO(string id);



