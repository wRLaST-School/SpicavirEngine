#include "stdafx.h"
#include "SrAlphaStage.h"
#include <Model.h>
#include <Camera.h>

void SrAlphaStage::Init()
{
#pragma region デフォルト3D
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

	scissorrect.left = 0;                                       // 切り抜き座標左
	scissorrect.right = scissorrect.left + GetSpWindow()->width;        // 切り抜き座標右
	scissorrect.top = 0;                                        // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + GetSpWindow()->height;       // 切り抜き座標下

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
	for (auto itr = sortedObj_.rbegin(); itr != sortedObj_.rend(); ++itr)
	{
		(*itr)->DrawCommands((*itr)->alphaTexKey);
	} 
	objects_.clear();
	sortedObj_.clear();
}

void SrAlphaStage::RegisterAlphaObject(Object3D* obj)
{
	objects_.push_back(obj);
}

void SrAlphaStage::SortObjects()
{
	Vec3 cam = Camera::sCurrent->position;
	for (auto o : objects_)
	{
		Vec3 v = cam - o->position;
		o->distanceToCam = v.GetSquaredLength();
	}

	sortedObj_.clear();

	for (auto o : objects_)
	{
		for (auto itr = sortedObj_.begin(); true; itr++)
		{
			if (itr == sortedObj_.end())
			{
				sortedObj_.push_back(o);
				break;
			}
			if ((*itr)->distanceToCam >= o->distanceToCam)
			{
				sortedObj_.insert(itr, o);
				break;
			}
		}
	}
}

void SrAlphaStage::DrawCommands(std::function<void(void)> cmd)
{
	OutputDebugStringA("Draw Command Queued on Unintended Stage\n");
}
