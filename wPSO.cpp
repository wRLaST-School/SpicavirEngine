#include "wPSO.h"
#include "wRootSignature.h"
map<string, wPSO> psoMap;

wPSO::wPSO(wShader* shader):shader(shader)
{
}

void wPSO::InitDesc()
{
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	psod.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	psod.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	psod.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	psod.PS.BytecodeLength = shader->psBlob->GetBufferSize();

	psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	psod.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	psod.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �J�����O
	psod.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	psod.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psod.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psod.NumRenderTargets = 1; // �`��Ώۂ�1��
	psod.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	psod.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//Depth
	psod.DepthStencilState.DepthEnable = true;
	psod.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psod.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psod.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}

void wPSO::InitDesc2D()
{
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	psod.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	psod.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	psod.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	psod.PS.BytecodeLength = shader->psBlob->GetBufferSize();

	psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	psod.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	psod.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psod.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �J�����O���Ȃ�

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psod.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psod.NumRenderTargets = 1; // �`��Ώۂ�1��
	psod.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	psod.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//Depth
	psod.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psod.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	psod.DepthStencilState.DepthEnable = false;
	psod.pRootSignature = GetRootSignature()->rootsignature.Get();
}

void wPSO::Create()
{
	HRESULT hr = GetWDX()->dev->CreateGraphicsPipelineState(&psod, IID_PPV_ARGS(&pso));
	int a = 0;
}

void RegisterAndInitPSO(string id, wShader* shader) 
{
	psoMap[id] = wPSO(shader);
	psoMap[id].InitDesc();
}

void RegisterAndInitPSOfor2D(string id, wShader* shader)
{
	psoMap[id] = wPSO(shader);
	psoMap[id].InitDesc2D();
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
