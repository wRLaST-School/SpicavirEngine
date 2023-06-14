#include "stdafx.h"
#include "SpDS.h"
#include <Sprite.h>
#include <RTVManager.h>
#include <SpSwapChainManager.h>
#include <GPipeline.h>
#include <SpRootSignature.h>

void SpDS::DrawRotaGraph(int32_t x, int32_t y, float dx, float dy, float rot, TextureKey key, Anchor anchor, Color brightness)
{
	Matrix m;
	auto meta = SpTextureManager::GetTextureMetadata(key);
	Float2 halfsize = { (float)meta.width / 2, (float)meta.height / 2 };

	//アンカーポイントに応じて移動
	float ancmX = 0;
	if (anchor == Anchor::TopLeft || anchor == Anchor::CenterLeft || anchor == Anchor::BottomLeft)
	{
		ancmX += halfsize.x;
	}
	else if (anchor == Anchor::TopRight || anchor == Anchor::CenterRight || anchor == Anchor::BottomRight)
	{
		ancmX -= halfsize.x;
	}
	float ancmY = 0;
	if (anchor == Anchor::TopLeft || anchor == Anchor::TopCenter || anchor == Anchor::TopRight)
	{
		ancmY += halfsize.y;
	}
	else if (anchor == Anchor::BottomLeft || anchor == Anchor::BottomCenter || anchor == Anchor::BottomRight)
	{
		ancmY -= halfsize.y;
	}
	m = Matrix();//Matrix::Translation({ancmX, ancmY, 0.f});

	m *= Matrix::Scale({ dx * halfsize.x * 2, dy * halfsize.y * 2, 1.f });

	m *= Matrix::RotZ(rot);

	m *= Matrix::Translation({ (float)x + ancmX * dx, (float)y + ancmY * dx, 0.f });

	Graph g;
	g.wMat = m;
	g.key = key;
	g.brightness = brightness;

	sGraphs.push_back(g);

	sGraphCount++;
}

void SpDS::DrawBox(int32_t x, int32_t y, int32_t width, int32_t height, float rot, const Color& color, const Anchor& anchor)
{
	DrawRotaGraph(x, y, (float)width, (float)height, rot, "white", anchor, color);
}

void SpDS::DrawBox(int32_t x0, int32_t y0, int32_t x1, int32_t y1, const  Color& color)
{
	int32_t sizeX = x1 - x0;
	int32_t sizeY = y1 - y0;

	DrawBox(x0 + sizeX / 2, y0 + sizeY / 2, sizeX, sizeY, 0.f, color);
}

void SpDS::SetBlendMode(const Blend& blendMode)
{
	switch (blendMode) {
	case Blend::Alpha:
		sCommands.insert(eastl::pair<int32_t, function<void(void)>>(sGraphCount, [&] {
			//パイプライン変更
			auto dx = GetWDX();

			dx->cmdList->SetPipelineState(GPipeline::GetState("2d"));
			dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("2D")->rootsignature.Get());
			}));
		break;

	case Blend::Sub:
		sCommands.insert(eastl::pair<int32_t, function<void(void)>>(sGraphCount, [&] {
			//パイプライン変更
			auto dx = GetWDX();

			dx->cmdList->SetPipelineState(GPipeline::GetState("2dSub"));
			dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("2D")->rootsignature.Get());
			}));
		break;

	case Blend::Add:
		sCommands.insert(eastl::pair<int32_t, function<void(void)>>(sGraphCount, [&] {
			//パイプライン変更
			auto dx = GetWDX();
			dx->cmdList->SetPipelineState(GPipeline::GetState("2dAdd"));
			dx->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("2D")->rootsignature.Get());
			}));

		break;
	}

}

void SpDS::SetRenderTarget(const TextureKey& key)
{
	sCommands.insert(
		eastl::pair<int32_t, function<void(void)>>(sGraphCount, [&, key] {
			//レンダーターゲット変更
			if (key == "CurrentBuffer")
			{
				RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
				return;
			}
			RTVManager::SetRenderTargetToTexture(key, false);
			}));
}

void SpDS::SetPreDrawFunc(function<void(void)> prop)
{
	sCommands.insert(eastl::pair<int32_t, function<void(void)>>(sGraphCount, prop));
}

void SpDS::DrawBoxLine(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color, float thickness, const  Anchor& anchor)
{
	anchor;
	int32_t x1 = x - width / 2;
	int32_t x2 = x + width / 2;
	int32_t y1 = y - height / 2;
	int32_t y2 = y + height / 2;

	DrawLine(x1, y1, x2, y1, color, (int32_t)thickness);
	DrawLine(x1, y1, x1, y2, color, (int32_t)thickness);
	DrawLine(x1, y2, x2, y2, color, (int32_t)thickness);
	DrawLine(x2, y1, x2, y2, color, (int32_t)thickness);
}

void SpDS::DrawLine(int32_t startX, int32_t startY, int32_t endX, int32_t endY, const Color& color, int32_t thickness)
{
	Vec2 ray((float)endX - (float)startX, (float)endY - (float)startY);
	float l = ray.GetLength();
	DrawBox(startX + (int32_t)(ray.x * 0.5f), startY + (int32_t)(ray.y * 0.5f), thickness, (int32_t)l, Vec2::VecToVec(Vec2(0.f, 1.f), ray.GetNorm()), color, Anchor::Center);
}

void SpDS::CreateBuffers()
{
	//graph用
	GraphVertData vertices[] = {
		{{-0.5, 0.5, 0}, {0.0f, 1.0f}},
		{{-0.5, -0.5, 0}, {0.0f, 0.0f}},
		{{0.5, 0.5, 0}, {1.0f, 1.0f}},
		{{0.5, -0.5, 0}, {1.0f, 0.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(GraphVertData) * _countof(vertices));

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
		IID_PPV_ARGS(&sGvertBuff)
	);

	sGvertBuff->SetName(L"DRAW GRAPH VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	GraphVertData* gvertMap = nullptr;
	sGvertBuff->Map(0, nullptr, (void**)&gvertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < _countof(vertices); i++)
	{
		gvertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	sGvertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	sGvbView.BufferLocation = sGvertBuff->GetGPUVirtualAddress();
	sGvbView.SizeInBytes = sizeVB;
	sGvbView.StrideInBytes = sizeof(GraphVertData);

	for (int32_t i = 0; i < GRAPH_BUFF_NUM; i++)
	{
		sGgpu.emplace_back();
	}
}

void SpDS::Render()
{
	RenderGraph();

	sGraphCount = 0;
}

void SpDS::RenderGraph()
{
	for (auto& ggp : sGgpu)
	{
		ggp.used = false;
	}

	auto ggpitr = sGgpu.begin();
	for (auto& g : sGraphs) {
		ggpitr->key = g.key;
		ggpitr->used = true;
		ggpitr->matcb.contents->mat = g.wMat * Sprite::sProj;
		ggpitr->matcb.contents->brightness = g.brightness;
		ggpitr++;
		if (ggpitr == sGgpu.end()) break;
	}
	sGraphs.clear();

	int32_t dGraphIndex = 0;
	for (auto& ggp : sGgpu)
	{
		if (!ggp.used) continue;

		//グラフインデックスに対応したコマンドがあったら実行
		for (auto& c : sCommands)
		{
			if (c.first == dGraphIndex)
			{
				c.second();
			}
		}

		//描画
		GetWDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(ggp.key));
		GetWDX()->cmdList->SetGraphicsRootConstantBufferView(0, ggp.matcb.buffer->GetGPUVirtualAddress());

		GetWDX()->cmdList->IASetVertexBuffers(0, 1, &sGvbView);

		GetWDX()->cmdList->DrawInstanced(4, 1, 0, 0);

		dGraphIndex++;
	}

	//グラフインデックスに対応したコマンドがあったら実行
	for (auto& c : sCommands)
	{
		if (c.first == dGraphIndex)
		{
			c.second();
		}
	}

	sCommands.clear();
}

eastl::vector<SpDS::Line> SpDS::sLines;

/*static D3D12_VERTEX_BUFFER_VIEW vbView;
static ComPtr<ID3D12Resource> vertBuff;*/
//static Line* vertMap;

eastl::vector<SpDS::Graph> SpDS::sGraphs;
eastl::list<SpDS::GraphGPUData> SpDS::sGgpu;
D3D12_VERTEX_BUFFER_VIEW SpDS::sGvbView;
ComPtr<ID3D12Resource> SpDS::sGvertBuff;
int32_t SpDS::sGraphCount = 0;

eastl::multimap<int32_t, function<void(void)>> SpDS::sCommands;