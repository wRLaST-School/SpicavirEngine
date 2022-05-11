#include "wPSO.h"
map<string, wPSO> psoMap;

wPSO::wPSO(wShader* shader):shader(shader)
{
}

void wPSO::InitDesc()
{
	// グラフィックスパイプライン設定
	psod.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	psod.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	psod.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	psod.PS.BytecodeLength = shader->psBlob->GetBufferSize();

	psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	psod.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	psod.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
	psod.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	psod.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	psod.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画

	psod.NumRenderTargets = 1; // 描画対象は1つ
	psod.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	psod.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//Depth
	psod.DepthStencilState.DepthEnable = true;
	psod.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psod.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psod.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}

void wPSO::Create()
{
	GetWDX()->dev->CreateGraphicsPipelineState(&psod, IID_PPV_ARGS(&pso));
}

void RegisterAndInitPSO(string id, wShader* shader) 
{
	psoMap[id] = wPSO(shader);
	psoMap[id].InitDesc();
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPSODesc(string id)
{
	return &psoMap[id].psod;
}

ID3D12PipelineState* GetPSO(string id)
{
	return psoMap[id].pso.Get();
}

wPSO* GetWPSO(string id)
{
	return &psoMap[id];
}
