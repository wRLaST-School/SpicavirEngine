#include "Sprite.h"
#include <fstream>
#include <sstream>
#include "wDirectX.h"
#include "wPSO.h"
#include "wRootSignature.h"

Sprite::Sprite(TextureKey key)
{
	this->tex = key;

	TexMetadata texmeta = wTextureManager::GetTextureMetadata(tex);
	this->width = texmeta.width;
	this->height = texmeta.height;
	float hlfw = (float)texmeta.width / 2;
	float hlfh = (float)texmeta.height / 2;
	Sprite::Vertex vertices[] = {
		{{-hlfw, hlfh, 0}, {0.0f, 1.0f}},
		{{-hlfw, -hlfh, 0}, {0.0f, 0.0f}},
		{{hlfw, hlfh, 0}, {1.0f, 1.0f}},
		{{hlfw, -hlfh, 0}, {1.0f, 0.0f}},
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

	constBuff.contents->color = {1.0, 1.0, 1.0, 1.0};
}

Sprite::Sprite(string path, TextureKey newKey)
{
	this->tex = wTextureManager::LoadTexture(path, newKey);

	TexMetadata texmeta = wTextureManager::GetTextureMetadata(tex);
	this->width = texmeta.width;
	this->height = texmeta.height;
	float hlfw = (float)texmeta.width / 2;
	float hlfh = (float)texmeta.height / 2;
	Sprite::Vertex vertices[] = {
		{{-hlfw, hlfh, 0}, {0.0f, 1.0f}},
		{{-hlfw, -hlfh, 0}, {0.0f, 0.0f}},
		{{hlfw, hlfh, 0}, {1.0f, 1.0f}},
		{{hlfw, -hlfh, 0}, {1.0f, 0.0f}},
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

	constBuff.contents->color = { 1.0, 1.0, 1.0, 1.0 };
}

void Sprite::PreSpriteDraw()
{
	ID3D12GraphicsCommandList* cl = GetWDX()->cmdList.Get();
	void* a = GetPSO("2d");
	cl->SetGraphicsRootSignature(GetRootSignature()->rootsignature.Get());
	cl->SetPipelineState(GetPSO("2d"));
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D12DescriptorHeap* ppHeaps[] = { wTextureManager::GetInstance().srvHeap.Get() };
	cl->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::PostSpriteDraw()
{
}

void Sprite::UpdateMatrix()
{
	world = Matrix::Identity();

	world *= Matrix::Scale(scale);
	world *= Matrix::RotZ(rot);
	world *= Matrix::Translation(position);

	constBuff.contents->mat = world;

	constBuff.contents->color = brightness;
}

void Sprite::Draw()
{
	constBuff.contents->mat = constBuff.contents->mat * proj;
	GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, wTextureManager::GetGPUDescHandle(tex));
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(2, this->constBuff.buffer->GetGPUVirtualAddress());

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &vbView);

	GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::InitCommon()
{
	proj = Matrix::Projection(GetwWindow()->width, GetwWindow()->height);
}


Matrix Sprite::proj;