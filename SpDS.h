#pragma once
#include <SpTextureManager.h>
#include <Anchor.h>
#include <Color.h>
#include <SpConstBuffer.h>
class SpDS
{
public:
	enum class Blend {
		Alpha,
		Add,
		Sub
	};

	static void DrawRotaGraph(int x, int y, float dx, float dy, float rot,
								TextureKey key, Anchor anchor = Anchor::Center,
									Color brightness = Color(0xffffff));

	static void DrawBox(int x, int y, int width, int height, float rot, Color color, Anchor anchor = Anchor::Center);
	
	static void SetBlendMode(Blend blendMode);
	static void SetRenderTarget(TextureKey key);
	static void SetPreDrawFunc(function<void(void)> prop);
	
	static void DrawBoxLine(int x, int y, int width, int height, float rot, Color color, Anchor anchor = Anchor::Center);
	static void DrawCircleLine(int x, int y, int r, Color color, int edges = 100);
	static void DrawLine(int startX, int startY, int endX, int endY, Color color, int thickness = 1);

	//各バッファと大量のダミースプライト初期化
	static void CreateBuffers();
	static void Render();

private:
	const static int graphBuffNum = 4096;
	const static int lineBuffNum = 2048;

	static void RenderGraph();

	struct Line {
		Float3 start;
		Float3 end;
		Color color;
	};

	struct Graph {
		Matrix wMat;
		TextureKey key;
		Color brightness;
		bool isRenderTarget = false;
	};

	struct GraphCBData {
		Float4 brightness;
		Matrix mat;
	};

	struct GraphGPUData {
		SpConstBuffer<GraphCBData> matcb;
		bool used = false;
		TextureKey key;
	};

	struct GraphVertData {
		Float3 pos;
		Float2 uv;
	};

	static eastl::vector<Line> lines;

	/*static D3D12_VERTEX_BUFFER_VIEW vbView;
	static ComPtr<ID3D12Resource> vertBuff;*/
	//static Line* vertMap;

	static eastl::vector<Graph> graphs;
	static eastl::list<GraphGPUData> ggpu;
	static D3D12_VERTEX_BUFFER_VIEW gvbView;
	static ComPtr<ID3D12Resource> gvertBuff;
	static int graphCount;
	static unordered_multimap<int, function<void(void)>> commands;
};

namespace SpDSLayouts {
	const static D3D12_INPUT_ELEMENT_DESC graphLayout[] = {
		{
				"POSITION", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
				"TEXCOORD", 0,
				DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// DrawLine2D用頂点レイアウト
	const static D3D12_INPUT_ELEMENT_DESC lineLayout[] = {
			{
				"FROM", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TO", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
	};
}
