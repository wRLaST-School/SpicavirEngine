#include "stdafx.h"
#include "SrAlphaStage.h"
#include <Model.h>
#include <Camera.h>

void SrAlphaStage::Init()
{
#pragma region �f�t�H���g3D
	PipelineDesc defDesc;
	defDesc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
	defDesc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);

	defDesc.RootSignature.ptr = SpRootSignature::Get("NoLight3D")->rootsignature.Get();

	defDesc.Shader.pShader = GetShader("lightless");
	defDesc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	defDesc.Depth.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	GPipeline::Create(defDesc, "Alpha3D");
#pragma endregion
}

void SrAlphaStage::PreDraw()
{
	SpDirectX* dx = GetWDX();
	dx->cmdList->SetPipelineState(GPipeline::GetState("Alpha3D"));
	dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("NoLight3D")->rootsignature.Get());

	D3D12_VIEWPORT viewport{};

	viewport.Width = (FLOAT)GetSpWindow()->width;
	viewport.Height = (FLOAT)GetSpWindow()->height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	dx->cmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorrect{};

	scissorrect.left = 0;                                       // �؂蔲�����W��
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // �؂蔲�����W�E
	scissorrect.top = 0;                                        // �؂蔲�����W��
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // �؂蔲�����W��

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	dx->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dx->cmdList->RSSetScissorRects(1, &scissorrect);

	Camera::UseCurrent();
}

void SrAlphaStage::PostDraw()
{
}

void SrAlphaStage::Render()
{
	for (auto itr = sortedObj.rbegin(); itr != sortedObj.rend(); ++itr)
	{
		(*itr)->DrawCommands((*itr)->alphaTexKey);
	} 
	objects.clear();
	sortedObj.clear();
}

void SrAlphaStage::RegisterAlphaObject(Object3D* obj)
{
	objects.push_back(obj);
}

void SrAlphaStage::SortObjects()
{
	if (!Camera::current) return;
	Vec3 cam = Camera::current->position;
	for (auto o : objects)
	{
		Vec3 v = cam - o->position;
		o->distanceToCam = v.GetSquaredLength();
	}

	sortedObj.clear();

	for (auto o : objects)
	{
		for (auto itr = sortedObj.begin(); true; itr++)
		{
			if (itr == sortedObj.end())
			{
				sortedObj.push_back(o);
				break;
			}
			if ((*itr)->distanceToCam >= o->distanceToCam)
			{
				sortedObj.insert(itr, o);
				break;
			}
		}
	}
}

void SrAlphaStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
