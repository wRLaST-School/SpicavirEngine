#include "stdafx.h"
#include "LineDrawer.h"
#include "SpDirectX.h"
#include <GPipeline.h>
#include <SpRootSignature.h>
#include <Camera.h>

void LineDrawer::Init()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Line) * MAX_LINES);

	////頂点バッファの設定
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
		IID_PPV_ARGS(&vertBuff_)
	);

	vertBuff_->SetName(L"LINE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(Line);
}

void LineDrawer::DrawLine(const Float3& start, const Float3& end, const Float4& color)
{
	lines_.emplace_back(start, end, color);
}

void LineDrawer::DrawLines(const vector<Float3>& positions, const Float4& color)
{
	for (auto itr = positions.begin(); itr + 1 != positions.end(); itr++)
	{
		lines_.emplace_back(*itr, *(itr + 1), color);
	}
}

void LineDrawer::DrawCube(const Float3& center, const Float3& scale, const Float4& color)
{
	vector<Float3> points = {
		(Vec3)center + Vec3(-scale.x,  scale.y, -scale.z),
		(Vec3)center + Vec3( scale.x,  scale.y, -scale.z),
		(Vec3)center + Vec3( scale.x, -scale.y, -scale.z),
		(Vec3)center + Vec3(-scale.x, -scale.y, -scale.z),
		(Vec3)center + Vec3(-scale.x,  scale.y,  scale.z),
		(Vec3)center + Vec3( scale.x,  scale.y,  scale.z),
		(Vec3)center + Vec3( scale.x, -scale.y,  scale.z),
		(Vec3)center + Vec3(-scale.x, -scale.y,  scale.z),
	};

	DrawLine(points[0], points[1], color);
	DrawLine(points[1], points[2], color);
	DrawLine(points[2], points[3], color);
	DrawLine(points[3], points[0], color);

	DrawLine(points[4], points[5], color);
	DrawLine(points[5], points[6], color);
	DrawLine(points[6], points[7], color);
	DrawLine(points[7], points[4], color);

	DrawLine(points[0], points[4], color);
	DrawLine(points[1], points[5], color);
	DrawLine(points[2], points[6], color);
	DrawLine(points[3], points[7], color);
}

void LineDrawer::DrawRotaCube(const Float3& center, const Float3& scale, const Float3& rot, const Float4& color)
{
	vector<Vec3> points = {
	(Vec3)center + Vec3(-scale.x,  scale.y, -scale.z),
	(Vec3)center + Vec3(scale.x,  scale.y, -scale.z),
	(Vec3)center + Vec3(scale.x, -scale.y, -scale.z),
	(Vec3)center + Vec3(-scale.x, -scale.y, -scale.z),
	(Vec3)center + Vec3(-scale.x,  scale.y,  scale.z),
	(Vec3)center + Vec3(scale.x,  scale.y,  scale.z),
	(Vec3)center + Vec3(scale.x, -scale.y,  scale.z),
	(Vec3)center + Vec3(-scale.x, -scale.y,  scale.z),
	};

	Matrix rotMatrix = Matrix::RotRollPitchYaw(rot);

	for (auto& p : points) {
		p *= rotMatrix;
	}

	DrawLine(points[0], points[1], color);
	DrawLine(points[1], points[2], color);
	DrawLine(points[2], points[3], color);
	DrawLine(points[3], points[0], color);

	DrawLine(points[4], points[5], color);
	DrawLine(points[5], points[6], color);
	DrawLine(points[6], points[7], color);
	DrawLine(points[7], points[4], color);

	DrawLine(points[0], points[4], color);
	DrawLine(points[1], points[5], color);
	DrawLine(points[2], points[6], color);
	DrawLine(points[3], points[7], color);
}

void LineDrawer::DrawAllLines()
{
	size_t index = 0;
	for (auto& l : lines_)
	{
		vertMap_[index].start = l.start;
		vertMap_[index].end = l.end;
		vertMap_[index].color = l.color;

		index++;
		if (index >= MAX_LINES) break;
	}

	GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("line"));
	GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("Particle")->rootsignature.Get());

	Camera::UseCurrent();

	GetWDX()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	GetWDX()->cmdList->IASetVertexBuffers(0, 1, &vbView_);

	GetWDX()->cmdList->DrawInstanced(static_cast<UINT>(lines_.size()), 1, 0, 0);
}

void LineDrawer::ClearLineData()
{
	lines_.clear();
}

D3D12_VERTEX_BUFFER_VIEW LineDrawer::vbView_{};
ComPtr<ID3D12Resource> LineDrawer::vertBuff_ = nullptr;
Line* LineDrawer::vertMap_ = nullptr;
vector<Line> LineDrawer::lines_;