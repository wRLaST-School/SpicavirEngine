#pragma once
#include "Essentials.h"
#include "wShader.h"
//TODO:ê›åvÇ™ÉKÉoÇ∑Ç¨ÇÈÇÃÇ≈pso,shader,rootsignature,textureÇÃê›åvÇå©íºÇ∑
class wPSO
{
public:
	wPSO() {};
	wPSO(wShader* shader);
	void InitDesc();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psod{};
	wShader* shader; 
	ComPtr<ID3D12PipelineState> pso = nullptr;
	void Create();
};

void RegisterAndInitPSO(string id, wShader* shader);
D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPSODesc(string id);
ID3D12PipelineState* GetPSO(string id);
wPSO* GetWPSO(string id);



