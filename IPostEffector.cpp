#include "stdafx.h"
#include "IPostEffector.h"
#include <GPipeline.h>
#include <SpRootSignature.h>
#include <Sprite.h>
#include <SpSwapChainManager.h>

void IPostEffector::RegisterPipeline(string name)
{
	RegisterShader(name);
	InitVS(name, name + "VS.hlsl");
	InitPS(name, name + "PS.hlsl");

	PipelineDesc pl2dDesc;
	pl2dDesc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
	pl2dDesc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);

	pl2dDesc.RootSignature.ptr = SpRootSignature::Get(name)->rootsignature.Get();

	pl2dDesc.Shader.pShader = GetShader(name);

	pl2dDesc.Depth.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pl2dDesc.Depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pl2dDesc.Depth.DepthStencilState.DepthEnable = false;
	pl2dDesc.Render.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pl2dDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	GPipeline::Create(pl2dDesc, name);
}

void IPostEffector::RegisterRS(string name)
{
#pragma region 2D Default RS
	{
		SpRootSignature* rs = SpRootSignature::Register(name);

		rs->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//�萔�o�b�t�@0�ԊePostEffect��p�f�[�^
		rs->params.emplace_back();
		rs->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs->params[0].Descriptor.ShaderRegister = 0;
		rs->params[0].Descriptor.RegisterSpace = 0;
		rs->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//�e�N�X�`�����W�X�^0��
		rs->params.emplace_back();
		rs->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rs->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rs->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rs->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rs->Create();
	}
#pragma endregion
}

void IPostEffector::Effect(TextureKey baseTex, TextureKey targetTex, string name)
{
	if (targetTex == "CurrentBuffer")
	{
		RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
	}
	else
	{
		RTVManager::SetRenderTargetToTexture(targetTex);
	}

	ID3D12GraphicsCommandList* cl = GetWDX()->cmdList.Get();
	cl->SetGraphicsRootSignature(SpRootSignature::Get(name)->rootsignature.Get());
	cl->SetPipelineState(GPipeline::GetState(name));
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorrect{};

	if (targetTex == "CurrentBuffer")
	{
		viewport.Width = GetSpWindow()->width;
		viewport.Height = GetSpWindow()->height;
		scissorrect.left = 0;                                       // �؂蔲�����W��
		scissorrect.right = scissorrect.left + viewport.Width;        // �؂蔲�����W�E
		scissorrect.top = 0;                                        // �؂蔲�����W��
		scissorrect.bottom = scissorrect.top + viewport.Height;       // �؂蔲�����W��
	}
	else
	{
		TexMetadata md = SpTextureManager::GetTextureMetadata(targetTex);
		viewport.Width = md.width;
		viewport.Height = md.height;
		scissorrect.left = 0;                                       // �؂蔲�����W��
		scissorrect.right = scissorrect.left + md.width;        // �؂蔲�����W�E
		scissorrect.top = 0;                                        // �؂蔲�����W��
		scissorrect.bottom = scissorrect.top + md.height;       // �؂蔲�����W��
	}

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GetWDX()->cmdList->RSSetViewports(1, &viewport);

	GetWDX()->cmdList->RSSetScissorRects(1, &scissorrect);

	ID3D12DescriptorHeap* ppHeaps[] = { SpTextureManager::GetInstance().srvHeap.Get() };
	cl->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(baseTex));
	//GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &PostEffectCommon::vbView);

	GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);
}

D3D12_VERTEX_BUFFER_VIEW PostEffectCommon::vbView{};

ComPtr<ID3D12Resource> PostEffectCommon::vertBuff = nullptr;

void PostEffectCommon::Init()
{
	Sprite::Vertex vertices[] = {
			{{-1, 1, 0}, {0.0f, 0.0f}},
			{{-1, -1, 0}, {0.0f, 1.0f}},
			{{1, 1, 0}, {1.0f, 0.0f}},
			{{1, -1, 0}, {1.0f, 1.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(Sprite::Vertex) * _countof(vertices));

	////���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	GetWDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"POST EFFECT VERT BUFF");
	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Sprite::Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}

	// �}�b�v������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Sprite::Vertex);
}
